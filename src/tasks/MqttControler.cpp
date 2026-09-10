//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "MqttControler.h"

#include "Log.h"

MqttControler* MqttControler_self = nullptr;

MqttControler::MqttControler(AppConfigMqtt* mqttconfig, AppEventQueue* outQueue) :
    AppTask("MqttControler", APP_TASK_STACK_DEFAULT, APP_TASK_PRIORITY_BACKEND, nullptr, outQueue),
    mqttClient(wifiClient)
{
    this->mqttconfig = mqttconfig;
    for (uint8_t i = 0; i < MQTT_MAX_SUBSCRIPTIONS; i++)
    {
        this->subscriptions[i] = nullptr;
    }

    MqttControler_self = this;
    this->mqttClient.setCallback(MqttControler::onMqttCallback);

    LOG_DEBUG("MqttControler::MqttControler");
}

MqttControler::~MqttControler()
{
    LOG_DEBUG("MqttControler::~MqttControler");
    this->disconnect();
    MqttControler_self = nullptr;
}

bool MqttControler::connect(uint16_t maxTries)
{
    if (this->mqttconfig == nullptr
        || this->mqttconfig->ready == false
        || this->mqttconfig->server[0] == '\0')
    {
        LOG_DEBUG("MqttControler::connect : mqtt not ready");
        return false;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        LOG_DEBUG("MqttControler::connect : wifi not connected");
        return false;
    }

    LOG_DEBUG("MqttControler::connect : mqtt ready");

    this->mqttClient.setServer(this->mqttconfig->server, MQTT_DEFAULT_PORT);
    this->mqttClient.setBufferSize(512);

    for (uint16_t i = 0; i < maxTries; i++)
    {
        try
        {
            LOG_DEBUG("MqttControler : Attempting to connect to server: %s", this->mqttconfig->server);
            bool connected = false;
            if (this->mqttconfig->username[0] != '\0')
            {
                connected = this->mqttClient.connect("energy-board", this->mqttconfig->username,
                                                     this->mqttconfig->password);
            }
            else
            {
                connected = this->mqttClient.connect("energy-board");
            }

            if (connected)
            {
                LOG_INFO("MqttControler : connected to %s", this->mqttconfig->server);
                this->resubscribe();
                return true;
            }

            LOG_ERROR("MqttControler : connect failed, state=%d", this->mqttClient.state());
            this->sleep(200);
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("MqttControler::connect : Exception standard capturee : %s", e.what());
        }
        catch (...)
        {
            LOG_ERROR("MqttControler::connect : Une exception inconnue s'est produite.");
        }
    }
    return false;
}

void MqttControler::resubscribe()
{
    for (uint8_t i = 0; i < this->subscriptionCount; i++)
    {
        if (this->subscriptions[i] != nullptr)
        {
            if (this->mqttClient.subscribe(this->subscriptions[i]))
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

    if (this->mqttClient.connected())
    {
        if (this->mqttClient.subscribe(topic))
        {
            LOG_INFO("MqttControler : subscribed to %s", topic);
            return true;
        }
        LOG_ERROR("MqttControler : subscribe failed for %s", topic);
        return false;
    }
    LOG_DEBUG("MqttControler::subscribe : deferred (not connected) %s", topic);
    return true;
}

void MqttControler::onMqttCallback(char* topic, byte* payload, unsigned int length)
{
    if (MqttControler_self == nullptr)
    {
        return;
    }

    MqttControler_self->mqttData.mqttTopic = String(topic);

    char buffer[512];
    unsigned int copyLen = (length < sizeof(buffer) - 1) ? length : sizeof(buffer) - 1;
    memcpy(buffer, payload, copyLen);
    buffer[copyLen] = '\0';
    MqttControler_self->mqttData.mqttPayload = String(buffer);

    LOG_DEBUG("MqttControler::onMqttCallback : %s = %s",
              MqttControler_self->mqttData.mqttTopic.c_str(),
              MqttControler_self->mqttData.mqttPayload.c_str());

    MqttControler_self->sendEvent(MqttControler_self->messageEvent);
}

void MqttControler::disconnect()
{
    if (this->mqttClient.connected())
    {
        this->mqttClient.disconnect();
    }
    this->status = MQTT_DO_NOTHING_STATE;
}

void MqttControler::run()
{
    while (true)
    {
        bool connected = this->mqttClient.connected();

        if (connected != (this->status == 1))
        {
            LOG_INFO("MqttControler : status change connected=%d", connected);
            this->status = connected ? 1 : 0;

            if (connected)
            {
                this->sendEvent(this->connectEvent);
            }
            else
            {
                this->sendEvent(this->discEvent);
            }
        }

        if (connected)
        {
            this->mqttClient.loop();
        }
        else if (WiFi.status() == WL_CONNECTED)
        {
            LOG_DEBUG("MqttControler : trying to connect");
            if (!this->connect(1))
            {
                LOG_ERROR("MqttControler : unable to connect");
            }
        }

        this->sleep(100);
    }
}
