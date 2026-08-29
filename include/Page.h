//
// Created by Patrick Rouillon on 04/02/2026.
//

#ifndef ENERGY_BOARD_PAGE_H
#define ENERGY_BOARD_PAGE_H

#include "Display.h"

#define DEFAULT_VERTICAL_SPACING 20
class Page
{
public:
    Page(Display& disp);
    virtual ~Page();
    virtual void create() = 0;
    virtual void show();
    virtual void hide();

protected:
    Display& display;
    lv_obj_t* page;  // each page is a LVGL Screen.
};


#endif //ENERGY_BOARD_PAGE_H
