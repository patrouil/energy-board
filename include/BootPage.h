//
// Created by Patrick Rouillon on 03/09/2026.
//

#pragma once

#ifndef ENERGY_BOARD_BOOTPAGE_H
#define ENERGY_BOARD_BOOTPAGE_H
#include "Screen.h"

class BootPage : public Screen
{
public:
    explicit BootPage() = default;

    ~BootPage() ;
    void create() override;
    void set_version(uint8_t  vers);

private:
    lv_obj_t* obj_label = nullptr;
};

#endif //ENERGY_BOARD_BOOTPAGE_H
