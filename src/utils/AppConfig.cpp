/*
 * config.c
 *
 *  Created on: Dec 13, 2025
 *      Author: patrick
 */

#include <string.h>
// must be first include
#include "PrivateConfig.h"
#include "AppConfig.h"
#include <FS.h>
#include "ArduinoYaml.h"
#include "Log.h"

static constexpr char WIFI_SID_KEY[] = "wifisid";
static constexpr char WIFI_PASSWORD_KEY[] = "wifikey";

static constexpr char MQTT_SERVER_KEY[] = "mqttserver";
static constexpr char MQTT_USERNAME_KEY[] = "username";
static constexpr char MQTT_PASSWORD_KEY[] = "password";
static constexpr char CONFIG_FILE_NAME[] = "energy.yaml";

const char* YAML_DEFAULTS = R"_YAML_STRING_(
wifi:
    wifisid: 7mar-guest
    wifikey: Welcome7
mqtt:
    mqttserver : broker.hivemq.com
    username :
    password :
subscription :
    consumption : home/status/consumption
    production : home/status/production
    lowRateMaxPower : home/status/lowRateMaxPower
    maxsolar : home/status/maxsolarpower
    maxgrid : home/status/maxgridpower
)_YAML_STRING_";

Preferences AppConfig::prefs;

bool AppConfig::loadConfigPref()
{
    prefs.begin("energy", false);
    if (!prefs.isKey(WIFI_SID_KEY))
    {
        return false;
    }
    prefs.getString(WIFI_SID_KEY, (this->wifi.sid), sizeof(this->wifi.sid) - 1);
    this->wifi.sid[sizeof(this->wifi.sid) - 1] = '\0';
    prefs.getString(WIFI_PASSWORD_KEY, (this->wifi.password), sizeof(this->wifi.password) - 1);
    this->wifi.sid[sizeof(this->wifi.password) - 1] = '\0';
    if (!prefs.isKey(MQTT_SERVER_KEY))
    {
        return false;
    }
    prefs.getString(MQTT_SERVER_KEY, (this->mqtt.server), sizeof(this->mqtt.server));
    this->wifi.sid[sizeof(this->mqtt.server) - 1] = '\0';
    prefs.getString(MQTT_USERNAME_KEY, (this->mqtt.username), sizeof(this->mqtt.username));
    this->wifi.sid[sizeof(this->mqtt.username) - 1] = '\0';
    prefs.getString(MQTT_PASSWORD_KEY, (this->mqtt.password), sizeof(this->mqtt.password));
    this->wifi.sid[sizeof(this->mqtt.password) - 1] = '\0';

    prefs.end();
    return true;
}

bool AppConfig::saveConfig() const
{
    prefs.putString(WIFI_SID_KEY, (this->wifi.sid));
    prefs.putString(WIFI_PASSWORD_KEY, (this->wifi.password));
    prefs.putString(MQTT_SERVER_KEY, (this->mqtt.server));
    prefs.putString(MQTT_USERNAME_KEY, (this->mqtt.username));
    prefs.putString(MQTT_PASSWORD_KEY, (this->mqtt.password));
    return true;
}

bool AppConfig::bindYaml(::YAMLNode& rootNode)
{
    try
    {
        YAMLNode wifi = rootNode["wifi"];
        const char* v;

        v = wifi[WIFI_SID_KEY].scalar();
        if (v) strncpy(this->wifi.sid, v, sizeof(this->wifi.sid));
        v = wifi[WIFI_PASSWORD_KEY].scalar();
        if (v) strncpy(this->wifi.password, v, sizeof(this->wifi.password));

        YAMLNode mqtt = rootNode["mqtt"];
        v = mqtt[MQTT_SERVER_KEY].scalar();
        if (v) strncpy(this->mqtt.server, v, sizeof(this->mqtt.server));
        v = mqtt[MQTT_USERNAME_KEY].scalar();
        if (v) strncpy(this->mqtt.username, v, sizeof(this->mqtt.username));
        v = mqtt[MQTT_PASSWORD_KEY].scalar();
        if (v) strncpy(this->mqtt.password, v, sizeof(this->mqtt.password));
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("AppConfig::bindYaml : Exception standard capturée : %s", e.what());
        return false;
    }
    catch (...)
    {
        LOG_ERROR("AppConfig::bindYaml : Une exception inconnue s'est produite.");
        return false;
    }
    return true;
}

bool AppConfig::loadDefaults()
{
    YAMLNode rootNode;
    int r = deserializeYml(rootNode, YAML_PRIVATE);
    this->bindYaml(rootNode);
    return true;
}

SdStatus AppConfig::loadConfigSD()
{
    LOG_DEBUG("AppConfig::loadConfigSD : ");

    YAMLNode rootNode;

    // le slot microSD est alimenté par le rail 3V3 principal (schéma spin 9),
    // pas par EXPANDER_POWER : la carte reste sous tension à travers les
    // reboots du board. Si elle a été interrompue en cours d'opération elle
    // reste "busy" (DO à 0) et ignore CMD0. SdFat n'appelle pas waitReady()
    // avant CMD0 : on horloge donc la carte CS bas jusqu'à relâche de DO.
    //
    // CS du LCD et du tactile explicitement hauts : avant dsp.init() le CS
    // du LCD (48) est flottant et le HX8357 peut piloter SDO (= MISO 41)
    pinMode(48, OUTPUT);
    digitalWrite(48, HIGH);
    pinMode(unPhone::SD_CS, OUTPUT);
    digitalWrite(unPhone::SD_CS, LOW);
    SPI.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
    // jusqu'à ~1 s d'horloge ; la carte signale ready par deux 0xFF
    uint8_t ready = 0;
    for (uint32_t i = 0; i < 50000 && ready < 2; i++)
    {
        ready = (SPI.transfer(0xFF) == 0xFF) ? ready + 1 : 0;
    }
    SPI.endTransaction();
    digitalWrite(unPhone::SD_CS, HIGH);
    LOG_DEBUG("AppConfig::loadConfigSD : busy recovery %s", ready >= 2 ? "ok" : "timeout");

    // Bus SPI déjà démarré (par le tactile dans unPhone::begin()) :
    // SdFat le réutilise sans le re-begin, sans jamais l'arrêter.
    SdSpiConfig cfg(
        unPhone::SD_CS, // CS dédié (43)
        SHARED_SPI | USER_SPI_BEGIN, // bus partagé, begin géré par l'utilisateur
        SD_SCK_MHZ(25), // 25 MHz comme la lib unPhone
        &SPI // instance explicite (FSPI, pins 39/40/41 du variant)
    );

    bool started = false;
    for (int attempt = 0; attempt < 3 && !started; attempt++)
    {
        started = sdcard.begin(cfg);
        if (!started)
        {
            LOG_ERROR("SD begin attempt %d failed: error=0x%X data=0x%X",
                      attempt + 1,
                      sdcard.card()->errorCode(),
                      sdcard.card()->errorData());
            delay(200);
        }
    }

    if (!started)
    {
        sdcard.errorPrint(&Serial);
        if (sdcard.card()->errorCode() != 0)
        {
            LOG_ERROR("AppConfig::loadConfigSD : no card or bad contact");
            return SdStatus::NO_CARD;
        }
        LOG_ERROR("AppConfig::loadConfigSD : wrong format (FAT32 expected)");
        return SdStatus::BAD_FORMAT;
    }
    LOG_DEBUG("AppConfig::loadConfigSD : ls");
    sdcard.ls();

    try
    {
        if (!sdcard.exists(CONFIG_FILE_NAME))
        {
            LOG_ERROR("AppConfig::loadConfigSD : no config file");
            return SdStatus::NO_FILE;
        }

        File32 fd = sdcard.open(CONFIG_FILE_NAME, O_READ);
        LOG_DEBUG("AppConfig::loadConfigSD fd is %d", fd.available32());
        // convert to stream.
        int r = deserializeYml(rootNode, fd);
        LOG_DEBUG("AppConfig::loadConfigSD : loaded bytes %d", r);
        this->bindYaml(rootNode);
        fd.close();
    }
    catch (std::runtime_error e)
    {
        LOG_ERROR("AppConfig::loadConfigSD : runtime error : %s", e.what());
        return SdStatus::READ_ERROR;
    }
    catch (...)
    {
        LOG_ERROR("AppConfig::loadConfigSD : exception");
        return SdStatus::READ_ERROR;
    }

    return SdStatus::OK;
}
