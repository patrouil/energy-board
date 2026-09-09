//
// Created by Patrick Rouillon on 23/01/2026.
//

#ifndef UPHONE1_APPEVENT_H
#define UPHONE1_APPEVENT_H

#include <stdint.h>


enum class AppEventType : uint8_t
{
    NONE =0 ,
    WIFI_IDLE = 10,
    WIFI_CONNECTED = 11,
    WIFI_DISCONNECTED = 12,
    UNPHONE_BUTTON1 = 20,
    UNPHONE_BUTTON2 = 21,
    UNPHONE_BUTTON3 = 22,
    UNPHONE_TOUCH_PRESSED = 30,
    UNPHONE_TOUCH_RELEASED = 31,
};

struct TouchPoint
{
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t z = 0;
};



class AppEvent
{
public:
    AppEvent(AppEventType id, const void* data);
    explicit AppEvent(AppEventType id);

    AppEventType getId() const;
    const void* getData() const;

private:
    AppEventType id = AppEventType::NONE;
    const void* data = nullptr;
};

#endif //UPHONE1_APPEVENT_H
