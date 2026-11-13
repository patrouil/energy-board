//
// Created by Patrick Rouillon on 25/01/2026.
//

#ifndef UPHONE1_MAINCONTROLER_H
#define UPHONE1_MAINCONTROLER_H
#include "AppTask.h"
#include "AppWifiControler.h"
#include "MqttControler.h"
#include "PageRouter.h"
#include "SimulationControler.h"
#include "UnphoneControler.h"
#include "ActivityManager.h"
#include "WarningManager.h"


enum class DataSource : uint8_t
{
    SOURCE_MQTT = 0,
    SOURCE_SIMUL,
    SOURCE_BOTH,
};


class AppMainControler : public AppTask
{
private:
    AppMainControler();
    ~AppMainControler() override;

    // WIFI section
    AppEventQueue* wifiOutQueue = nullptr;
    AppWifiControler* wifiManager = nullptr;
    //
    bool manage_wifi_event();
    bool manage_unphone_event();
    bool manage_mqtt_event();
    void manageEnergyEvent(const EventData& event_data);
    bool manage_simulation_event();

    // MQTT section
    AppEventQueue* mqttOutQueue = nullptr;
    MqttControler* mqttManager = nullptr;

    // simulation section
    AppEventQueue* simulationOutQueue = nullptr;
    SimulationControler* simulationManager = nullptr;

    // unphone section
    AppEventQueue* unphoneOutQueue = nullptr;
    UnphoneControler* unphoneManager = nullptr;

    ActivityManager  activityManager;
    ScreenId currentScreen = ScreenId::BOOT_PAGE;
    DataSource dataSource = DataSource::SOURCE_MQTT;
    WarningManager warningManager;

    EventData eventDataImage;

public:
    static AppMainControler& getInstance()
    {
        static AppMainControler instance;
        return instance;
    }

    void setup();
    void subscribeMqtt(const char* topic);

    AppWifiControler* get_wifi_controler() const
    {
        return wifiManager;
    }

    MqttControler* get_mqtt_controler() const
    {
        return mqttManager;
    }

    UnphoneControler* get_unphone_controler() const
    {
        return unphoneManager;
    }

    SimulationControler* get_simulation_controler() const
    {
        return simulationManager;
    }

    void run() override;
};


#endif //UPHONE1_MAINCONTROLER_H
