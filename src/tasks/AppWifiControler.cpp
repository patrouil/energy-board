//
// Created by Patrick Rouillon on 18/01/2026.
//


#include <WiFi.h>
#include <AppWifiControler.h>
#include "AppConfig.h"

#include "AppTask.h"
#include "app_event_queue.h"

AppWifiControler::AppWifiControler(AppConfigWifi* wificonfig, AppEventQueue* outQueue) :
    AppTask("AppWifiControler", APP_TASK_STACK_DEFAULT, APP_TASK_PRIORITY_BACKEND, nullptr, outQueue)
{
    this->wificonfig = wificonfig;

    LOG_DEBUG("AppWifiControler::AppWifiControler");
}


AppWifiControler::~AppWifiControler()
{
    LOG_DEBUG("AppWifiControler::AppWifiControler");
    this->disconnect();
}

bool AppWifiControler::connect(uint16_t maxTries)
{
    if (this->wificonfig == nullptr
        || this->status == WIFI_DO_NOTHING_STATE)
    {
        LOG_DEBUG("AppWifiControler::connect : wifi not ready %s",
                  (this->wificonfig == nullptr) ? "???" : this->wificonfig->sid);
        return false;
    }
    LOG_DEBUG("AppWifiControler::connect : wifi  ready");

    for (uint16_t i = 0; i < maxTries; i++)
    {
        try
        {
            LOG_DEBUG("AppWifiControler : Attempting to connect to WPA SSID: %s / %s", this->wificonfig->sid,
                      this->wificonfig->password);
            WiFi.begin(this->wificonfig->sid, this->wificonfig->password);
            LOG_INFO("AppWifiControler : done begin");
            for (uint8_t j = 0; j < 20; j++)
            {
                // 20 * 500ms = 10s
                this->sleep(100);
                if (WiFi.status() == WL_CONNECTED && WiFi.localIP() != INADDR_NONE)
                {
                    wifiData.ipAddress = WiFi.localIP().toString();
                    LOG_INFO("WiFi connected! IP: %s", wifiData.ipAddress.c_str());
                    return true;
                }
            }
            WiFi.disconnect();
            wifiData.ipAddress = "";

            LOG_ERROR("AppWifiControler : login error");
            this->sleep(100);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("AppWifiControler::connect : Exception standard capturée : %s", e.what());
        }
        catch (...)
        {
            LOG_ERROR("AppWifiControler::connect : Une exception inconnue s'est produite.");
        }
    }
    return false;
}

void AppWifiControler::disconnect()
{
    WiFi.disconnect();
    wifiData.ipAddress = "";
    this->status = WL_DISCONNECTED;
}

void AppWifiControler::run()
{
    while (true)
    {
        //checkStack();

        uint8_t s = WiFi.status();
        if (s != this->status) // status change
        {
            LOG_INFO("AppWifiControler : new status is %d", s);
            this->status = s;
            switch (s)
            {
            case WL_NO_SHIELD:
            case WL_IDLE_STATUS:
                LOG_INFO("AppWifiControler : send idle event");
                this->sendEvent(this->idleEvent);
                break;
            case WL_CONNECTED:
                this->sendEvent(this->connectEvent);
                break;
            case WL_CONNECT_FAILED:
                LOG_ERROR("AppWifiControler : connect failed %d", s);
                break;
            case WL_DISCONNECTED:
            case WL_CONNECTION_LOST:
                LOG_ERROR("AppWifiControler : connection lost  %d", s);
                this->sendEvent(this->discEvent);
                break;
            case WL_NO_SSID_AVAIL:
            case WL_SCAN_COMPLETED:
            default:
                LOG_ERROR("AppWifiControler : unknown status %d", s);
            }
        }
        else if (this->status != WL_CONNECTED)
        {
            LOG_DEBUG("AppWifiControler : trying to connect");
            if (!this->connect(1))
            {
                LOG_ERROR("AppWifiControler : unable to connect");
            }
        }

        this->sleep(3000);
    }
}
