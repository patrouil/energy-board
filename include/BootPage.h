//
// Created by Patrick Rouillon on 03/09/2026.
//

#ifndef ENERGY_BOARD_BOOTPAGE_H
#define ENERGY_BOARD_BOOTPAGE_H
#include "Screen.h"
#include "unPhone.h"


class BootPage : public Screen
{
public:
    explicit BootPage(Display& disp)
        : Screen(disp)
    {
    }

    ~BootPage() override ;
    void create() override;
    void create(unPhone &phone);

private:
    lv_obj_t* obj_label = nullptr;
};


#endif //ENERGY_BOARD_BOOTPAGE_H
