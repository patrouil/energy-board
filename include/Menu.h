//
// Created by Patrick Rouillon on 04/02/2026.
//

#ifndef ENERGY_BOARD_MENU_H
#define ENERGY_BOARD_MENU_H

#include <vector>
#include "Screen.h"

class Menu {
public:
    Menu(lv_obj_t* parent);
    void addPage(Screen* page, const char* name);
    void show();

private:
    lv_obj_t* menuContainer;
    std::vector<Screen*> windows;
};




#endif //ENERGY_BOARD_MENU_H