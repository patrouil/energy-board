//
// Created by @patrouil on 10/11/2026.
// with the assistance of Mistral Vibe (https://github.com/mistral-vibe).
//

#include "ActivityManager.h"

#include "Log.h"


void ActivityManager::setProductionPower(int32_t power)
{
    if (power < productionPower && power <= MIN_POWER_TRIGGER)
    {
        productionPower = power;
        timeStamp = time(nullptr); // start inactive timer.
    }
    else if ( power > MIN_POWER_TRIGGER)
    {
        productionPower = power;
        timeStamp = 0;  // activate.
    }
    time_t t = time(nullptr);
    //LOG_DEBUG("ActivityManager::setProductionPower power %d  stamp %d  clock %d  movestamp %d", power, timeStamp, t, movedStamp);
}

void ActivityManager::setDeviceMoved(bool moved)
{
    //LOG_DEBUG("ActivityManager::setDeviceMoved moved %d", moved);
    if (moved)
        movedStamp = time(nullptr);
}

/*
 * active means : we are in the day (there is a solar production)
 *  or someone moved the device
 */
bool ActivityManager::isActive()
{
    time_t now = time(nullptr);
    return (timeStamp == 0) || (now <= timeStamp + TIMETOLIVE)
        || (now <= movedStamp + TIMETOLIVE);
}
