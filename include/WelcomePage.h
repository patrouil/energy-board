//
// Created by Patrick Rouillon on 04/02/2026.
//

#ifndef ENERGY_BOARD_WELCOMEPAGE_H
#define ENERGY_BOARD_WELCOMEPAGE_H

#include "Screen.h"

class WelcomePage : public Screen
{
    lv_obj_t* titleLabel;
    lv_obj_t* ipLabel;
    lv_obj_t* mqttLabel;
    lv_obj_t* messageLabel;

public:
    explicit WelcomePage(Display& display);
    ~WelcomePage() ;

    void create() override;

    void setIPAddress(const char* ip);
    void setMQTTStatus(const char* status);
    void setMessage(const char* message);
};

#endif //ENERGY_BOARD_WELCOMEPAGE_H
