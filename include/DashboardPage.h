//
// Created by Patrick Rouillon on 11/09/2026.
//

#pragma once

#ifndef ENERGY_BOARD_DASHBOARDPAGE_H
#define ENERGY_BOARD_DASHBOARDPAGE_H

#include "Screen.h"

class DashboardPage : public Screen
{
    lv_obj_t* oneLabel = nullptr;
    lv_obj_t* lastLabel = nullptr;

public:
    DashboardPage() = default;
    ~DashboardPage();

    void create() override;
};

#endif //ENERGY_BOARD_DASHBOARDPAGE_H
