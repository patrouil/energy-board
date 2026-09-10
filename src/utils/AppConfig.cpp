/*
 * config.c
 *
 *  Created on: Dec 13, 2025
 *      Author: patrick
 */



// #include <stdio.h>
#include <string.h>

// #include <unPhone.h>

#include "AppConfig.h"


static constexpr char WIFI_SID_KEY[] = "wifisid";
static constexpr char WIFI_PASSWORD_KEY[] = "wifikey";

static constexpr char MQTT_SERVER_KEY[] = "mqttserver";
static constexpr char MQTT_USERNAME_KEY[] = "username";
static constexpr char MQTT_PASSWORD_KEY[] = "password";

Preferences AppConfig::prefs;

bool AppConfig::loadConfig()
{
    prefs.begin("energy", false);
    if (!prefs.isKey(WIFI_SID_KEY))
    {
        this->wifi.ready = false;
        return false;
    }
    prefs.getString(WIFI_SID_KEY, (this->wifi.sid), sizeof(this->wifi.sid)-1);
    this->wifi.sid[sizeof(this->wifi.sid)-1] = '\0';
    prefs.getString(WIFI_PASSWORD_KEY, (this->wifi.password), sizeof(this->wifi.password)-1);
    this->wifi.sid[sizeof(this->wifi.password)-1] = '\0';
    if (!prefs.isKey(MQTT_SERVER_KEY))
    {
        this->mqtt.ready = false;
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

void AppConfig::defaultWifi()
{
    strcpy(wifi.sid, "7mar-home");
    strcpy(wifi.password, "0951793067");
    wifi.ready = true;
    this->saveConfig();
}

void AppConfig::defaultMqtt()
{
    strcpy(mqtt.server, "broker.hivemq.com");
    mqtt.username[0] = '\0';
    mqtt.password[0] = '\0';
    mqtt.ready = true;
    this->saveConfig();
}
