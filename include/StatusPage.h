//
// Created by Patrick Rouillon on 04/02/2026.
//

#pragma once

#ifndef ENERGY_BOARD_STATUSPAGE_H
#define ENERGY_BOARD_STATUSPAGE_H

#include "Screen.h"

class StatusPage : public Screen
{
    static constexpr uint8_t MAX_MESSAGES = 5;
    static constexpr uint8_t MESSAGE_MAX_LEN = 40;

    lv_obj_t* titleLabel = nullptr;
    lv_obj_t* wifiSSIDLabel = nullptr;
    lv_obj_t* ipLabel = nullptr;
    lv_obj_t* mqttServerLabel = nullptr;
    lv_obj_t* mqttLabel = nullptr;
    lv_obj_t* messageLabel = nullptr;

    char messageLines[MAX_MESSAGES][MESSAGE_MAX_LEN + 1];
    char messageBuffer[MAX_MESSAGES * (MESSAGE_MAX_LEN + 1) + 1];
    uint8_t messageCount = 0;

public:
    StatusPage() = default;
    ~StatusPage() ;

    void create() override;

    void setWifiSSID(const char* ssid);
    void setIPAddress(const char* ip);
    void setMqttServer(const char* server);
    void setMQTTStatus(const char* status);
    void setMessage(const char* message);
    void addMessage(const char* message);

private:
    void updateMessageBuffer();
};

#endif //ENERGY_BOARD_STATUSPAGE_H
