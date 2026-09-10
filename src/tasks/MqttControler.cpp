//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "MqttControler.h"

#include <WiFi.h>

#include "Log.h"

MqttControler::MqttControler(AppConfigMqtt* mqttconfig, AppEventQueue* outQueue) :
    AppTask("MqttControler", APP_TASK_STACK_DEFAULT, APP_TASK_PRIORITY_BACKEND, nullptr, outQueue)
{
    this->mqttconfig = mqttconfig;
    for (uint8_t i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++)
    {
        this->subscriptions[i] = nullptr;
    }

    LOG_DEBUG("MqttControler::MqttControler");
}

MqttControler::~MqttControler()
{
    LOG_DEBUG("MqttControler::~MqttControler");
    this->disconnect();
}

void MqttControler::resubscribe()
{
    for (uint8_t i = 0; i < this->subscriptionCount; i++)
    {
        if (this->subscriptions[i] != nullptr)
        {
            int msgId = esp_mqtt_client_subscribe(this->mqttClient, this->subscriptions[i], 0);
            if (msgId >= 0)
            {
                LOG_INFO("MqttControler : subscribed to %s", this->subscriptions[i]);
            }
            else
            {
                LOG_ERROR("MqttControler : subscribe failed for %s", this->subscriptions[i]);
            }
        }
    }
}

bool MqttControler::subscribe(const char* topic)
{
    if (topic == nullptr || topic[0] == '\0')
    {
        LOG_ERROR("MqttControler::subscribe : invalid topic");
        return false;
    }

    if (this->subscriptionCount >= MQTT_MAX_SUBSCRIPTIONS)
    {
        LOG_ERROR("MqttControler::subscribe : subscription table full");
        return false;
    }

    this->subscriptions[this->subscriptionCount] = topic;
    this->subscriptionCount++;

    if (this->mqttClient != nullptr)
    {
        int msgId = esp_mqtt_client_subscribe(this->mqttClient, topic, 0);
        if (msgId >= 0)
        {
            LOG_INFO("MqttControler : subscribed to %s", topic);
            return true;
        }
        LOG_ERROR("MqttControler : subscribe failed for %s", topic);
        return false;
    }
    LOG_DEBUG("MqttControler::subscribe : deferred (not started) %s", topic);
    return true;
}

void MqttControler::onMqttEvent(void* handler_arg, esp_event_base_t base, int32_t event_id,
                                 void* event_data)
{
    MqttControler* self = static_cast<MqttControler*>(handler_arg);
    if (self == nullptr || event_data == nullptr)
    {
        return;
    }

    esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        LOG_INFO("MqttControler : MQTT_EVENT_CONNECTED");
        self->resubscribe();
        self->sendEvent(self->connectEvent);
        break;
    case MQTT_EVENT_DISCONNECTED:
        LOG_INFO("MqttControler : MQTT_EVENT_DISCONNECTED");
        self->sendEvent(self->discEvent);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        LOG_DEBUG("MqttControler : MQTT_EVENT_SUBSCRIBED msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        {
            if (event->topic != nullptr && event->data != nullptr)
            {
                self->mqttData.mqttTopic = String(event->topic, event->topic_len);
                self->mqttData.mqttPayload = String(event->data, event->data_len);

                LOG_DEBUG("MqttControler::MQTT_EVENT_DATA : %s = %s",
                          self->mqttData.mqttTopic.c_str(),
                          self->mqttData.mqttPayload.c_str());

                self->sendEvent(self->messageEvent);
            }
        }
        break;
    case MQTT_EVENT_ERROR:
        LOG_ERROR("MqttControler : MQTT_EVENT_ERROR type=%d", event->error_handle->error_type);
        break;
    default:
        break;
    }
}

void MqttControler::disconnect()
{
    if (this->mqttClient != nullptr)
    {
        esp_mqtt_client_stop(this->mqttClient);
        esp_mqtt_client_destroy(this->mqttClient);
        this->mqttClient = nullptr;
    }
}

void MqttControler::run()
{
    while (true)
    {
        if (this->mqttClient == nullptr
            && WiFi.status() == WL_CONNECTED
            && this->mqttconfig != nullptr
            && this->mqttconfig->ready
            && this->mqttconfig->server[0] != '\0')
        {
            esp_mqtt_client_config_t config = {};
            config.host = this->mqttconfig->server;
            config.port = MQTT_DEFAULT_PORT;
            config.client_id = "energy-board";

            if (this->mqttconfig->username[0] != '\0')
            {
                config.username = this->mqttconfig->username;
                config.password = this->mqttconfig->password;
            }

            config.disable_auto_reconnect = false;
            config.network_timeout_ms = 10000;

            this->mqttClient = esp_mqtt_client_init(&config);
            if (this->mqttClient == nullptr)
            {
                LOG_ERROR("MqttControler : esp_mqtt_client_init failed");
            }
            else
            {
                esp_err_t reg = esp_mqtt_client_register_event(this->mqttClient,
                                                               MQTT_EVENT_ANY,
                                                               MqttControler::onMqttEvent,
                                                               this);
                if (reg != ESP_OK)
                {
                    LOG_ERROR("MqttControler : register_event failed %d", reg);
                    esp_mqtt_client_destroy(this->mqttClient);
                    this->mqttClient = nullptr;
                }
                else
                {
                    esp_err_t start = esp_mqtt_client_start(this->mqttClient);
                    if (start != ESP_OK)
                    {
                        LOG_ERROR("MqttControler : client_start failed %d", start);
                        esp_mqtt_client_destroy(this->mqttClient);
                        this->mqttClient = nullptr;
                    }
                    else
                    {
                        LOG_INFO("MqttControler : client started on %s", this->mqttconfig->server);
                    }
                }
            }
        }

        this->sleep(2000);
    }
}
