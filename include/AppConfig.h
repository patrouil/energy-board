/*
 * config.h
 *
 *  Created on: Dec 13, 2025
 *      Author: patrick
 */

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
    static Preferences prefs;
    unPhone *board;

public:

    static AppConfig *appConfig;

    AppConfigWifi wifi;
    AppConfigMqtt mqtt;

    AppConfig(unPhone *board);
    ~AppConfig() = default;

    bool loadConfig();
    bool saveConfig() const;
    static Preferences & getPreferences()
    {
        return prefs;
    }

    void defaultWifi();

};
#endif /* CONFIG_H_ */
