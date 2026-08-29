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
