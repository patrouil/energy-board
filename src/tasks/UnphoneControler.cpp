//
// Created by Patrick Rouillon on 09/09/2026.
// with the assistance of Mistral Vibe Code.
//

#include <Wire.h>
#include <Adafruit_LSM6DS.h>
#include "unPhone.h"
#include "Log.h"
#include "UnphoneControler.h"

#include "Display.h"
#include "lv_api_map.h"
#include <cmath>


UnphoneControler::UnphoneControler(AppEventQueue* outQueue) :
    ::AppTask("AppUnphoneControler", APP_TASK_STACK_DEFAULT, APP_TASK_PRIORITY_BACKEND, nullptr, outQueue)
{
    //  LOG_DEBUG("UnphoneControler::UnphoneControler");
}

bool UnphoneControler::handleButtonPress()
{
    unPhone* ph = unPhone::me;
    int currentButton = 0;

    try
    {
        currentButton |= (ph->button1()) ? EventData::BUTTON_1 : 0;
        currentButton |= (ph->button2()) ? EventData::BUTTON_2 : 0;
        currentButton |= (ph->button3()) ? EventData::BUTTON_3 : 0;
        // LOG_DEBUG("UnphoneControler::handleButtonPress : currentButton = %x", currentButton);
        if (currentButton != this->unPhoneData.currentButton)
        {
            this->unPhoneData.currentButton = currentButton;

            if (currentButton != 0)
            {
                this->sendEvent(this->buttonPressed);
            }
            else
            {
                this->sendEvent(this->buttonRelease);
            }
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("handleButtonPress : known exception : %s", e.what());
    }
    catch (...)
    {
        LOG_ERROR("handleButtonPress : uncatched exception");
    }
    return (currentButton != 0);
}

bool UnphoneControler::handleAccelerator()
{
    sensors_event_t accel;

    unPhone::me->getAccelEvent(&accel);
    float magnitude_square =
        std::pow(accel.acceleration.x, 2) + std::pow(accel.acceleration.y, 2) + std::pow(accel.acceleration.z, 2);
    if (magnitude_square > 100.0) // earth magitude is close to 9
    {
        // LOG_DEBUG("UnphoneControler::handleAccelerator magnitude %f", magnitude_square);
        if ((this->outgoingQueue->isEmpty())) // do not send twice.
        {
            this->unPhoneData.movedDevice = true;
            this->sendEvent(this->movedEvent);
        }
    }
    else
        this->unPhoneData.movedDevice = false;
    return (magnitude_square > 100.0);
}

    // map touch coords to lcd coords
    // a version of map that never returns out of range values
    long UnphoneControler::positionMapper(long x, long in_min, long in_max, long out_min, long out_max)
    {
        long probable =
            (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        if (probable < out_min) return out_min;
        if (probable > out_max) return out_max;
        return probable;
    }


    /*Read the touchpad*/
    void UnphoneControler::touchPadRead(lv_indev_drv_t* indev_driver, lv_indev_data_t* data)
    {
        uint16_t touchX, touchY;
        unPhone* ph = unPhone::me;
        Display& disp = Display::getInstance();

        LOG_DEBUG("UnphoneControler::touchPadRead");

        if (ph->tsp == nullptr) return;
        // start of changes for unPhone ////////////////////////////////////////////

        if (!ph->tsp->touched())
        {
            data->state = LV_INDEV_STATE_REL;
        }
        else
        {
            data->state = LV_INDEV_STATE_PR;

            /*Set the coordinates*/
            TS_Point p(-1, -1, -1);
            p = ph->tsp->getPoint();
            LOG_DEBUG("UnphoneControler::touchPadRead: p.x(%04d), p.y(%04d) p.z(%04d)", p.x, p.y, p.z);
            // filter the ghosting on version 9 boards (on USB power; ~300 pressure)
#if UNPHONE_SPIN >= 9
            if (p.z < 400) return;
#endif
            if (p.x < 0 || p.y < 0)
                LOG_ERROR("UnphoneControler::touchPadRead: less than zero!");

            long xMin = 320;
            long xMax = 3945;
            long yMin = 420;
            long yMax = 3915;

            uint16_t displayWidth = disp.displayWidth;
            uint16_t displayHeight = disp.displayHeight;

            long xscld = UnphoneControler::positionMapper((long)p.x, xMin, xMax, 0, (long)displayWidth);
            long yscld = // Y is inverted on rotation 1 (landscape, buttons right)
                static_cast<long>(displayHeight) -
                UnphoneControler::positionMapper((long)p.y, yMin, yMax, 0, (long)displayHeight);
            touchX = (uint16_t)xscld;
            touchY = (uint16_t)yscld;

            LOG_DEBUG("touchX(%4d), touchY(%4d)", touchX, touchY);
            // end of changes for unPhone ////////////////////////////////////////////

            data->point.x = touchX;
            data->point.y = touchY;
            LOG_DEBUG("Data x %u, Data y %u", touchX, touchY);
        }
    }

    void UnphoneControler::vibe()
    {
        unPhone::me->vibe(true);
        this->vibeTimeStamp = time(nullptr);
    }


    void UnphoneControler::setup()
    {
        static lv_indev_drv_t indev_drv;
        // LOG_DEBUG("UnphoneControler::setup");
        /*Initialize the (dummy) input device driver*/
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = UnphoneControler::touchPadRead;
        // NOT YET lv_indev_drv_register(&indev_drv);
        // use the unphone accelerator API
    }


    void UnphoneControler::run()
    {
        sleep(2000); // at boot time
        while (true)
        {
            checkStack();

            if (this->vibeTimeStamp > 0 && time(nullptr) > this->vibeTimeStamp)
            {
                this->vibeTimeStamp = 0;
                unPhone::me->vibe(false);
            }

            if ( handleButtonPress() ||
                handleAccelerator() )
            {
                sleep(50);
            } else
            {
                sleep(100);
            }

            // float v = unPhone::me->batteryVoltage();
            unPhone::me->checkPowerSwitch();
        }
    }
