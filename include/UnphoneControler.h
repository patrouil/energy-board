//
// Created by Patrick Rouillon on 04/02/2026.
//

#ifndef ENERGY_BOARD_UNPHONECONTROLER_H
#define ENERGY_BOARD_UNPHONECONTROLER_H

#include <unPhone.h>

#include "AppTask.h"
#include "app_event.h"

#define UNPHONE_CONTROLER_STACK_SIZE (4 * 1024)

class UnphoneControler : public AppTask
{
private:
    static UnphoneControler* controler;

    unPhone* board = nullptr;

    uint8_t currentButton = 0xFF;
    bool touchActive = false;

    TouchPoint lastTouch;

    void manage_buttons();
    void manage_touch();

public:
    UnphoneControler(unPhone* board, AppEventQueue* outQueue);
    ~UnphoneControler() override;

    void setup() override;

    unPhone* get_board() const
    {
        return this->board;
    }

public:
    void run() override;
};

#endif //ENERGY_BOARD_UNPHONECONTROLER_H
