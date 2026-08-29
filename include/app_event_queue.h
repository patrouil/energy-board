//
// Created by Patrick Rouillon on 24/01/2026.
//

#ifndef UPHONE1_APP_QUEUE_H
#define UPHONE1_APP_QUEUE_H

#include <queue.h>
#include <freertos/portmacro.h>
#include "app_event.h"


#define APP_EVENT_QUEUE_SMALL_SIZE 3
#define APP_EVENT_QUEUE_DEFAULT_SIZE 8

class AppEventQueue
{
public:
    explicit AppEventQueue(uint8_t length, TickType_t ticksToWait = portMAX_DELAY);

    ~AppEventQueue();
    bool push(const AppEvent& event) const;
    bool pop(AppEvent* event) const;

    bool isEmpty() const;
    bool isFull() const;
    void setMSToWait(const uint32_t milliSec);
    void setTicksToWait(const TickType_t& ticks_to_wait);

private:
    QueueHandle_t queueHandle;
    TickType_t ticksToWait;
};


#endif //UPHONE1_APP_QUEUE_H
