/*
 * config.h
 *
 *  Created on: Dec 13, 2025
 *      Author: patrick
 */
#pragma once


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

enum class SdStatus : uint8_t
{
    OK = 0,
    NO_CARD,
    BAD_FORMAT,
    NO_FILE,
    READ_ERROR,
};

class AppConfig
{
private :
    SdFat sdcard;

    bool bindYaml(::YAMLNode &rootNode);

protected :
    AppConfig() ;
    ~AppConfig() = default;

public:

    static AppConfig& getInstance()
    {
        static AppConfig instance;
        return instance;
    }

    AppConfigWifi wifi;
    AppConfigMqtt mqtt;

    bool loadDefaults();
    void setSPIBus();

    SdStatus loadConfigSD();


};
