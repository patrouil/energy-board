//
// Created by Patrick Rouillon on 09/09/2026.
//

#pragma once

#include "Adafruit_LSM6DS33.h"
#include "app_event.h"
#include "app_event_queue.h"
#include "EventData.h"
#include "AppTask.h"
#include "hal/lv_hal_indev.h"


class UnphoneControler : public AppTask
{
private :
    bool handleButtonPress();
    bool handleAccelerator();
    static void touchPadRead(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);
    static long positionMapper(long x, long in_min, long in_max, long out_min, long out_max);

    EventData unPhoneData;

    const AppEvent buttonPressed = AppEvent(AppEventType::UNPHONE_BUTTON_PRESSED, unPhoneData);
    const AppEvent buttonRelease = AppEvent(AppEventType::UNPHONE_BUTTON_RELEASED, unPhoneData);
    const AppEvent movedEvent = AppEvent(AppEventType::UNPHONE_MOVED, unPhoneData);

     const uint8_t LSM6DS_I2C_ADDR = 0x6B;   // SA0 haut sur unPhone 9
     const uint8_t LSM6DS_INT_PIN  = 4;

    long vibeTimeStamp = 0;
    bool accelEvent = false;

public :
    UnphoneControler(AppEventQueue* outQueue);
    ~UnphoneControler() = default;

    void run() override;
    void setup();
    void vibe();

};
