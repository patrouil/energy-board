//
// Created by Patrick Rouillon on 11/09/2026.
//

#pragma once

#include "PowerBar.h"
#include "Screen.h"

class DashboardPage : public Screen
{
    lv_obj_t* oneLabel = nullptr;
    PowerBar powerBar;

    lv_obj_t* bottomMessage = nullptr;

    lv_obj_t* create_ticker(lv_obj_t *parent);

public:
    DashboardPage() = default;
    ~DashboardPage();

    void create() override;

    void setBottomMessage(const char* message);

    void setPowerRange(int32_t min, int32_t max)
    {
        powerBar.setRange(min, max);
    }

    void setProductionValue(int32_t production, int32_t powerAvailable)
    {
        powerBar.setProductionValue(production);
        powerBar.setCheapValue(powerAvailable);
    }

};
