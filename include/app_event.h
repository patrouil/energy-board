//
// Created by Patrick Rouillon on 23/01/2026.
//

#ifndef UPHONE1_APPEVENT_H
#define UPHONE1_APPEVENT_H

#include <stdint.h>

#define APP_EVENT_NONE 0
#define APP_EVENT_WIFI_IDLE 10
#define APP_EVENT_WIFI_CONNECTED 11
#define APP_EVENT_WIFI_DISCONNECTED 12
#define APP_EVENT_UNPHONE_BUTTON1 20
#define APP_EVENT_UNPHONE_BUTTON2 21
#define APP_EVENT_UNPHONE_BUTTON3 22


class AppEvent
{
public:
    AppEvent(int16_t id, const void* data);
    explicit AppEvent(int16_t id);

    int16_t getId() const;
    const void* getData() const;

private:
    int16_t id = APP_EVENT_NONE;
    const void* data = nullptr;
};

#endif //UPHONE1_APPEVENT_H
