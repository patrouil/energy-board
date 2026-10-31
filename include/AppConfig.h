/*
 * config.h
 *
 *  Created on: Dec 13, 2025
 *      Author: patrick
 */
#pragma once


#include <Preferences.h>

#include <unPhone.h>

#include "ArduinoYaml.hpp"

typedef struct {
    char sid[64];
    char password[64];
    char hostname[64];
} AppConfigWifi;

typedef struct {
    char server[64];
    char username[32];
    char password[64];
} AppConfigMqtt;

typedef struct {
    char production[64];
    char lowRateMaxPower[64];
    char maxsolar[64];
    char maxgrid[64];
} AppConfigMqttEntries;


/* extern func */

class AppConfig
{
    bool bindYaml(::YAMLNode &rootNode);

protected :
    AppConfig() = default;
    ~AppConfig() = default;

    static Preferences prefs;

public:

    static AppConfig& getInstance()
    {
        static AppConfig instance;
        return instance;
    }

    AppConfigWifi wifi;
    AppConfigMqtt mqtt;

    bool loadDefaults();
    bool loadConfigPref();
    bool saveConfig() const;

    bool loadConfigSD();


};
