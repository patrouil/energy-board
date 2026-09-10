/*
 * config.h
 *
 *  Created on: Dec 13, 2025
 *      Author: patrick
 */
#pragma once

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Preferences.h>

#include <unPhone.h>

typedef struct {
    bool ready = false;
    char sid[64];
    char password[64];
    char hostname[64];
} AppConfigWifi;

typedef struct {
    bool ready = false;
    char server[64];
    char username[32];
    char password[64];
} AppConfigMqtt;


/* extern func */

class AppConfig
{
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

    bool loadConfig();
    bool saveConfig() const;

    void defaultWifi();
    void defaultMqtt();

};
#endif /* CONFIG_H_ */
