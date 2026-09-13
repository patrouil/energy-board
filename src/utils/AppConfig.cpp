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

const char * YAML_DEFAULTS =  R"_YAML_STRING_(
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
    poweravailable : home/status/poweravailable
    maxproduction : home/status/maxproduction
    maxconsumption : home/status/maxconsumption
)_YAML_STRING_";

Preferences AppConfig::prefs;

bool AppConfig::loadConfigPerf()
{
    prefs.begin("energy", false);
    if (!prefs.isKey(WIFI_SID_KEY))
    {
        return false;
    }
    prefs.getString(WIFI_SID_KEY, (this->wifi.sid), sizeof(this->wifi.sid)-1);
    this->wifi.sid[sizeof(this->wifi.sid)-1] = '\0';
    prefs.getString(WIFI_PASSWORD_KEY, (this->wifi.password), sizeof(this->wifi.password)-1);
    this->wifi.sid[sizeof(this->wifi.password)-1] = '\0';
    if (!prefs.isKey(MQTT_SERVER_KEY))
    {
        return false;
    }
    prefs.getString(MQTT_SERVER_KEY, (this->mqtt.server), sizeof(this->mqtt.server));
    this->wifi.sid[sizeof(this->mqtt.server)-1] = '\0';
    prefs.getString(MQTT_USERNAME_KEY, (this->mqtt.username), sizeof(this->mqtt.username));
    this->wifi.sid[sizeof(this->mqtt.username)-1] = '\0';
    prefs.getString(MQTT_PASSWORD_KEY, (this->mqtt.password), sizeof(this->mqtt.password));
    this->wifi.sid[sizeof(this->mqtt.password)-1] = '\0';

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

bool AppConfig::bindYaml(::YAMLNode &rootNode)
{
    try
    {
        YAMLNode wifi = rootNode["wifi"];
        const char *v;

        v = wifi[WIFI_SID_KEY].scalar();
        if ( v ) strncpy(this->wifi.sid ,v, sizeof(this->wifi.sid));
        v = wifi[WIFI_PASSWORD_KEY].scalar();
        if ( v ) strncpy(this->wifi.password , v, sizeof(this->wifi.password));

        YAMLNode mqtt = rootNode["mqtt"];
        v = mqtt[MQTT_SERVER_KEY].scalar();
        if ( v ) strncpy(this->mqtt.server, v, sizeof(this->mqtt.server));
        v = mqtt[MQTT_USERNAME_KEY].scalar();
        if ( v ) strncpy(this->mqtt.username, v, sizeof(this->mqtt.username));
        v = mqtt[MQTT_PASSWORD_KEY].scalar();
        if ( v ) strncpy(this->mqtt.password, v, sizeof(this->mqtt.password));
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
    int r =  deserializeYml( rootNode,  YAML_PRIVATE);
    this->bindYaml(rootNode);
    return true;
}
bool AppConfig::loadConfigSD()
{

    unPhone *ph = unPhone::me;
    SdFat * sdc = ph->sdp;
    YAMLNode rootNode;

    APP_ASSERT(sdc != nullptr);
    if ( sdc == nullptr )
        return false;

    File32 fd = sdc->open(CONFIG_FILE_NAME, O_READ);

    // convert to stream.
    int r =  deserializeYml( rootNode,  fd);
    fd.close();
    return false;
}
