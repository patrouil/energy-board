//
// Created by Patrick Rouillon on 04/02/2026.
//

#pragma once

#ifndef ENERGY_BOARD_WELCOMEPAGE_H
#define ENERGY_BOARD_WELCOMEPAGE_H

#include "Screen.h"

class WelcomePage : public Screen
{
    lv_obj_t* titleLabel = nullptr;
    lv_obj_t* ipLabel = nullptr;
    lv_obj_t* mqttLabel = nullptr;
    lv_obj_t* messageLabel = nullptr;

public:
    WelcomePage() = default;
    ~WelcomePage() ;

    void create() override;

    void setIPAddress(const char* ip);
    void setMQTTStatus(const char* status);
    void setMessage(const char* message);
};

#endif //ENERGY_BOARD_WELCOMEPAGE_H
