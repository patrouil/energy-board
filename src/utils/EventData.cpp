//
// Created by Patrick Rouillon on 08/09/2026.
//

#include "EventData.h"



// locked only for 1sec max (this is long)
void EventData::lock()
{
    xSemaphoreTake(logMutex, portTICK_PERIOD_MS * 1000 );
}

void EventData::release()
{
    xSemaphoreGive(logMutex);

}

void EventData::atomic(void(*func)() )
{
    lock();
    func();
    release();
}
