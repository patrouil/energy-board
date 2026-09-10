//
// Created by Patrick Rouillon on 04/02/2026.
//

#pragma once

#ifndef UPHONE1_MQTTCONTROLER_H
#define UPHONE1_MQTTCONTROLER_H

#include "mqtt_client.h"

#include "AppConfig.h"
#include "AppTask.h"
#include "EventData.h"

#define MQTT_DEFAULT_PORT 1883
#define MQTT_MAX_SUBSCRIPTIONS 8

class MqttControler : public AppTask
{
    AppConfigMqtt* mqttconfig = nullptr;
    esp_mqtt_client_handle_t mqttClient = nullptr;

    const char* subscriptions[MQTT_MAX_SUBSCRIPTIONS];
    uint8_t subscriptionCount = 0;

    EventData mqttData;

    const AppEvent discEvent = AppEvent(AppEventType::MQTT_DISCONNECTED);
    const AppEvent connectEvent = AppEvent(AppEventType::MQTT_CONNECTED, mqttData);
    const AppEvent messageEvent = AppEvent(AppEventType::MQTT_MESSAGE_RECEIVED, mqttData);

    void resubscribe();

    static void onMqttEvent(void* handler_arg, esp_event_base_t base, int32_t event_id,
                            void* event_data);

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
