/*
 * config.c
 *
 *  Created on: Dec 13, 2025
 *      Author: patrick
 */

#include <stdio.h>
#include <string.h>
//#include <ctype.h>
//#include <stdbool.h>
//#include <ff_studio.h>
#include <unPhone.h>

#include "AppConfig.h"

#include "Log.h"

static const char WIFI_SID_KEY[] = "wifisid";
static const char WIFI_PASSWORD_KEY[] = "wifikey";

static const char MQTT_SERVER_KEY[] = "mqttserver";
static const char MQTT_USERNAME_KEY[] = "username";
static const char MQTT_PASSWORD_KEY[] = "password";

AppConfig* AppConfig::appConfig = nullptr;
Preferences AppConfig::prefs;

AppConfig::AppConfig(unPhone* board):board(board)
{
    this->appConfig = this;

}

bool AppConfig::loadConfig()
{
    prefs.begin("energy", false);
    if (!prefs.isKey(WIFI_SID_KEY))
    {
        this->wifi.ready = false;
        return false;
    }
    prefs.getString(WIFI_SID_KEY, (this->wifi.sid), sizeof(this->wifi.sid)-1);
    prefs.getString(WIFI_PASSWORD_KEY, (this->wifi.password), sizeof(this->wifi.password)-1);
    if (!prefs.isKey(MQTT_SERVER_KEY))
    {
        this->mqtt.ready = false;
        return false;
    }
    prefs.getString(MQTT_SERVER_KEY, (this->mqtt.server), sizeof(this->mqtt.server)-1);
    prefs.getString(MQTT_USERNAME_KEY, (this->mqtt.username), sizeof(this->mqtt.username)-1);
    prefs.getString(MQTT_PASSWORD_KEY, (this->mqtt.password), sizeof(this->mqtt.password)-1);
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
