//
// Created by Patrick Rouillon on 04/10/2026.
//

#pragma once

#ifndef ENERGY_BOARD_WATTHELPER_H
#define ENERGY_BOARD_WATTHELPER_H
#include <lvgl.h>

class wattHelper
{

public :
    static int32_t MAX( int32_t a, int32_t b) {return (a > b ? a : b);};
    static const char * WATT_UNIT ;
    static const char * KILOWATT_UNIT ;

    /* input
    *
    * watts power in watts.
    * valueBuffer pointer to buffer where the label will be stored. (must be at least 12 char long)
    * valueBufferSize size of the buffer.
    * unit pointer to buffer where the unit will be stored. (must be at least 4 char long)
    * return a point to the uinit string
    */
    static const char *labelOf(int32_t watts, char * valueBuffer, int32_t valueBufferSize);

};


#endif //ENERGY_BOARD_WATTHELPER_H
