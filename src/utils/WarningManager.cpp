//
// Created by @patrouil on 11/11/2026.
// with the assistance of Mistral Vibe (https://github.com/mistral-vibe).
//

#include "WarningManager.h"

#include "Log.h"


void WarningManager::setPower(int32_t homeConsuption, int32_t lowRatePower)
{
    bool previous = highConsuptionState;
    highConsuptionState = ( lowRatePower > MIN_LOW_RATE) && (homeConsuption > lowRatePower);
    // too much consuption and previously not.
    // so warn the threshold have been raised.
    if (  highConsuptionState && !previous)
        notifySent = false;
}

bool WarningManager::isWarning()
{
    LOG_DEBUG("WarningManager::isWarning state %d  sent %d", highConsuptionState, notifySent);
    // set warning if consumprion too high and alert not sent yet.
    bool ret = highConsuptionState && !notifySent;
    notifySent = true;
    return ret;
}
