//
// Created by Patrick Rouillon on 09/09/2026.
//

#pragma once

#include "app_event.h"
#include "app_event_queue.h"
#include "EventData.h"
#include "AppTask.h"
#include "hal/lv_hal_indev.h"


class UnphoneControler : public AppTask
{
private :
    void handleButtonPress();
    static void touchPadRead(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
    static long positionMapper(long x, long in_min, long in_max, long out_min, long out_max);

    EventData unPhoneData;

    const AppEvent buttonPressed = AppEvent(AppEventType::UNPHONE_BUTTON_PRESSED, unPhoneData);
    const AppEvent buttonRelease = AppEvent(AppEventType::UNPHONE_BUTTON_RELEASED, unPhoneData);

public :
    UnphoneControler(AppEventQueue* outQueue);
    ~UnphoneControler() = default;

    void run() override;
    void setup();
};
