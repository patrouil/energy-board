//
// Created by Patrick Rouillon on 04/02/2026.
//

#ifndef ENERGY_BOARD_PAGE_H
#define ENERGY_BOARD_PAGE_H

#include "Display.h"

#define DEFAULT_VERTICAL_SPACING 20

class Screen
{
public:
    Screen(Display& disp);
    ~Screen();
    virtual void create() ;
    virtual void show();
    virtual void hide();

protected:
    const Display& display;
    lv_obj_t* page;  // each page is a LVGL Screen.
};


#endif //ENERGY_BOARD_PAGE_H
