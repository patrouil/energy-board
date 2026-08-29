//
// Created by Patrick Rouillon on 18/01/2026.
//

#ifndef UPHONE1_WIFI_MANAGER_H
#define UPHONE1_WIFI_MANAGER_H

#include <WiFi.h>

#include "AppConfig.h"
#include "AppTask.h"

#define DEFAULT_MAX_RETRY 5
#define WIFI_DO_NOTHING_STATE (WL_NO_SHIELD-1)
#define APP_WIFI_CONTROLER_STACK_SIZE (5*1024)

class AppWifiControler : public AppTask
{
    AppConfigWifi* wificonfig;
    uint8_t status = WIFI_DO_NOTHING_STATE;

    const AppEvent idleEvent = AppEvent(APP_EVENT_WIFI_IDLE);
    const AppEvent discEvent = AppEvent(APP_EVENT_WIFI_DISCONNECTED);
    const AppEvent connectEvent = AppEvent(APP_EVENT_WIFI_CONNECTED);

    bool connect(uint16_t maxTries = DEFAULT_MAX_RETRY);

public:
    AppWifiControler(AppConfigWifi* wificonfig, AppEventQueue* outQueue);
    ~AppWifiControler() override = default;

    AppConfigWifi* get_wificonfig() const
    {
        return this->wificonfig;
    }

    void set_wificonfig(AppConfigWifi* const wificonfig)
    {
        this->wificonfig = wificonfig;
    }

    void disconnect();
    void setup();

public:
    void run() override;
};


#endif //UPHONE1_WIFI_MANAGER_H
