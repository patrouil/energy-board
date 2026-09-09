//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "UnphoneControler.h"

#include "Log.h"
#include "Display.h"

UnphoneControler* UnphoneControler::controler = nullptr;

static long unphone_map(long x, long in_min, long in_max, long out_min, long out_max)
{
    long probable = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    if (probable < out_min) return out_min;
    if (probable > out_max) return out_max;
    return probable;
}

UnphoneControler::UnphoneControler(unPhone* board, AppEventQueue* outQueue) :
    AppTask("unphoneControler", UNPHONE_CONTROLER_STACK_SIZE, APP_TASK_PRIORITY_BUSINESS, nullptr, outQueue)
{
    this->board = board;
    UnphoneControler::controler = this;
    LOG_DEBUG("UnphoneControler::UnphoneControler");
}

UnphoneControler::~UnphoneControler()
{
    LOG_DEBUG("UnphoneControler::~UnphoneControler");
    this->board = nullptr;
}

void UnphoneControler::setup()
{
    LOG_DEBUG("UnphoneControler::setup");
    APP_ASSERT(this->board != nullptr);
    APP_ASSERT(this->outgoingQueue != nullptr);
}

void UnphoneControler::manage_buttons()
{
    APP_ASSERT(this->board != nullptr);
    unPhone& ph = *this->board;

    try
    {
        if (ph.button1() && this->currentButton != unPhone::BUTTON1)
        {
            const AppEvent ev(AppEventType::UNPHONE_BUTTON1);
            this->sendEvent(ev);
            this->currentButton = unPhone::BUTTON1;
            LOG_DEBUG("UnphoneControler::BUTTON1");
        }
        else if (ph.button2() && this->currentButton != unPhone::BUTTON2)
        {
            const AppEvent ev(AppEventType::UNPHONE_BUTTON2);
            this->sendEvent(ev);
            this->currentButton = unPhone::BUTTON2;
            LOG_DEBUG("UnphoneControler::BUTTON2");
        }
        else if (ph.button3() && this->currentButton != unPhone::BUTTON3)
        {
            const AppEvent ev(AppEventType::UNPHONE_BUTTON3);
            this->sendEvent(ev);
            this->currentButton = unPhone::BUTTON3;
            LOG_DEBUG("UnphoneControler::BUTTON3");
        }
        else
        {
            this->currentButton = 0xFF;
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("UnphoneControler::manage_buttons : exception : %s", e.what());
    }
    catch (...)
    {
        LOG_ERROR("UnphoneControler::manage_buttons : unknown exception");
    }
}

void UnphoneControler::manage_touch()
{
    APP_ASSERT(this->board != nullptr);
    unPhone& ph = *this->board;

    try
    {
        if (!ph.tsp->touched())
        {
            if (this->touchActive)
            {
                const AppEvent ev(AppEventType::UNPHONE_TOUCH_RELEASED);
                this->sendEvent(ev);
                this->touchActive = false;
                LOG_DEBUG("UnphoneControler::TOUCH_RELEASED");
            }
            return;
        }

        TS_Point p = ph.tsp->getPoint();

#if UNPHONE_SPIN >= 9
        if (p.z < 400) return;
#endif

        if (p.x < 0 || p.y < 0) return;

        const long xMin = 320;
        const long xMax = 3945;
        const long yMin = 420;
        const long yMax = 3915;

        uint16_t x = (uint16_t)unphone_map((long)p.x, xMin, xMax, 0, (long)displayWidth);
        uint16_t y = (uint16_t)((long)displayHeight -
                                unphone_map((long)p.y, yMin, yMax, 0, (long)displayHeight));

        this->lastTouch.x = x;
        this->lastTouch.y = y;
        this->lastTouch.z = p.z;

        const AppEvent ev(AppEventType::UNPHONE_TOUCH_PRESSED, &this->lastTouch);
        this->sendEvent(ev);
        this->touchActive = true;
        LOG_DEBUG("UnphoneControler::TOUCH_PRESSED x=%d y=%d z=%d", x, y, p.z);
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("UnphoneControler::manage_touch : exception : %s", e.what());
    }
    catch (...)
    {
        LOG_ERROR("UnphoneControler::manage_touch : unknown exception");
    }
}

void UnphoneControler::run()
{
    LOG_DEBUG("UnphoneControler::run");
    while (true)
    {
        manage_buttons();
        manage_touch();
        this->sleep(50);
    }
}
