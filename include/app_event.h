//
// Created by Patrick Rouillon on 23/01/2026.
//

#pragma once

#ifndef UPHONE1_APPEVENT_H
#define UPHONE1_APPEVENT_H

#include <stdint.h>

#include "EventData.h"

enum class AppEventType : uint8_t
{
    NONE =0 ,
    WIFI_IDLE = 10,
    WIFI_CONNECTED = 11,
    WIFI_DISCONNECTED = 12,
    UNPHONE_BUTTON_PRESSED = 20,
    UNPHONE_BUTTON_RELEASED = 21,
    LAST_EVENT_TYPE,
};

class AppEvent
{
public:
    AppEvent(AppEventType id, EventData & data);
    explicit AppEvent(AppEventType id);

    AppEventType getId() const;
    const EventData& getData() const;

private:
    AppEventType id = AppEventType::NONE;
    const EventData& data;
};

#endif //UPHONE1_APPEVENT_H
