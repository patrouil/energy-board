//
// Created by @patrouil on 11/11/2026.
// with the assistance of Mistral Vibe (https://github.com/mistral-vibe).
//


#pragma once
#include <cstdint>


class WarningManager
{
    const int32_t MIN_LOW_RATE = 1000;  // below 1kwh do not care.

    bool highConsuptionState = false;
    bool notifySent = true;
public :
    void setPower(int32_t homeConsuption, int32_t lowRatePower);
    bool isWarning();
};
