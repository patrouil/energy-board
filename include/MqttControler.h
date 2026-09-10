//
// Created by Patrick Rouillon on 04/02/2026.
//

#pragma once

#ifndef UPHONE1_MQTTCONTROLER_H
#define UPHONE1_MQTTCONTROLER_H

#include <WiFiClient.h>
#include <PubSubClient.h>

#include "AppConfig.h"
#include "AppTask.h"
#include "EventData.h"

#define MQTT_DEFAULT_PORT 1883
#define MQTT_DEFAULT_RETRY 5
#define MQTT_DO_NOTHING_STATE (-1)
#define MQTT_MAX_SUBSCRIPTIONS 8

class MqttControler : public AppTask
{
    AppConfigMqtt* mqttconfig = nullptr;
    int8_t status = MQTT_DO_NOTHING_STATE;

    WiFiClient wifiClient;
    PubSubClient mqttClient;

    const char* subscriptions[MQTT_MAX_SUBSCRIPTIONS];
    uint8_t subscriptionCount = 0;

    EventData mqttData;

    const AppEvent idleEvent = AppEvent(AppEventType::MQTT_IDLE);
    const AppEvent discEvent = AppEvent(AppEventType::MQTT_DISCONNECTED);
    const AppEvent connectEvent = AppEvent(AppEventType::MQTT_CONNECTED, mqttData);
    const AppEvent messageEvent = AppEvent(AppEventType::MQTT_MESSAGE_RECEIVED, mqttData);

    bool connect(uint16_t maxTries = MQTT_DEFAULT_RETRY);
    void resubscribe();

    static void onMqttCallback(char* topic, byte* payload, unsigned int length);

public:
    MqttControler(AppConfigMqtt* mqttconfig, AppEventQueue* outQueue);
    ~MqttControler() override;

    AppConfigMqtt* get_mqttconfig() const
    {
        return this->mqttconfig;
    }

    void set_mqttconfig(AppConfigMqtt* const mqttconfig)
    {
        this->mqttconfig = mqttconfig;
    }

    bool subscribe(const char* topic);
    void disconnect();

    void run() override;
};

#endif //UPHONE1_MQTTCONTROLER_H
