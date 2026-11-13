//
// Created by @patrouil on 10/11/2026.
// with the assistance of Mistral Vibe (https://github.com/mistral-vibe).
//

#pragma once

#include <ctime>


class ActivityManager
{
    const int32_t TIMETOLIVE = 1800; // delay in sec.
    const int32_t MIN_POWER_TRIGGER = 10;

    uint32_t timeStamp = time(nullptr);
    int32_t productionPower = 0;
    uint32_t movedStamp = time(nullptr);

public :
    bool isActive();

    void setProductionPower(int32_t power);

    void setDeviceMoved(bool moved);
};
