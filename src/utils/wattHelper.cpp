//
// Created by Patrick Rouillon on 04/10/2026.
//

#include "wattHelper.h"


#include "Log.h"

const char* wattHelper::WATT_UNIT = "Wh";
const char* wattHelper::KILOWATT_UNIT = "kWh";

const char* wattHelper::labelOf(int32_t watts, char* valueBuffer, int32_t valueBufferSize)
{
    //LOG_DEBUG("wattHelper::labelOf");

    const char* unit;
    int32_t absWatts = (watts < 0) ? -watts : watts;

    if ( absWatts == 0)
    {
        snprintf(valueBuffer, valueBufferSize, "--.-");
        unit = WATT_UNIT;
    }
    else if (absWatts < 1000)
    {
        snprintf(valueBuffer, valueBufferSize, "%d", (int)watts);
        unit = WATT_UNIT;
    }
    else if (absWatts < 10000)
    {
        int32_t kilo = absWatts / 1000;
        int32_t centi = (absWatts % 1000) / 10;
        snprintf(valueBuffer, valueBufferSize, "%s%d.%02d",
                 (watts < 0) ? "-" : "", kilo, centi);
        unit = KILOWATT_UNIT;
    }
    else // > 10000
    {
        int32_t kilo = absWatts / 1000;
        int32_t deci = (absWatts % 1000) / 100;
        snprintf(valueBuffer, valueBufferSize, "%s%d.%01d",
                 (watts < 0) ? "-" : "", kilo, deci);
        unit = KILOWATT_UNIT;
    }
    return unit;
}
