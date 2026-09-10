//
// Created by Patrick Rouillon on 24/01/2026.
//

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "app_event_queue.h"

#include "Log.h"

AppEventQueue::AppEventQueue(uint8_t length, TickType_t ticksToWait)
{
    this->ticksToWait = ticksToWait;
    this->queueHandle = xQueueCreate(length, sizeof(AppEvent));
    if ( this->queueHandle == 0 )
        LOG_ERROR("AppEventQueue::AppEventQueue: queue not created");
}

AppEventQueue::~AppEventQueue()
{
    vQueueDelete(queueHandle);
}

bool AppEventQueue::push(const AppEvent& event) const
{
    if ( queueHandle == NULL ) return false;
    LOG_DEBUG("AppEventQueue::push: %x ", &event);

    return xQueueSend(queueHandle, (void *)&event, this->ticksToWait) == pdPASS;
}

bool AppEventQueue::pop(AppEvent * event) const
{
    if ( queueHandle == NULL ) return false;
    LOG_DEBUG("AppEventQueue::pop: %x", event);

    return xQueueReceive(queueHandle, event, this->ticksToWait) == pdPASS;
}

bool AppEventQueue::isEmpty() const
{
    return uxQueueMessagesWaiting(queueHandle) == 0;
}

bool AppEventQueue::isFull() const
{
    if ( queueHandle == NULL ) return true;

    return uxQueueSpacesAvailable(queueHandle) == 0;
}

void AppEventQueue::setTicksToWait(const TickType_t& ticks_to_wait)
{
    ticksToWait = ticks_to_wait;
}

void AppEventQueue::setMSToWait(const uint32_t milliSec)
{
    ticksToWait = pdMS_TO_TICKS(milliSec);
}
