//
// Created by Patrick Rouillon on 11/09/2026.
//

#pragma once

#include "PowerBar.h"
#include "PowerCard.h"
#include "PowerScale.h"
#include "Screen.h"
#include "Theme.h"

class DashboardPage : public Screen
{
    lv_obj_t* oneLabel = nullptr;
    PowerBar powerBar;
    PowerScale powerScale;
    PowerCard powerCards[3];

    lv_obj_t* bottomMessage = nullptr;

    static const lv_coord_t CARDS_SIDE_MARGIN = 10;
    static const lv_coord_t CARDS_GAP = 4;

    lv_obj_t* create_power_cards(lv_obj_t *parent);

    lv_obj_t* create_ticker(lv_obj_t *parent);

public:
    DashboardPage() = default;
    ~DashboardPage();

    void create() override;

    void setBottomMessage(const char* message);

    void setPowerRange(int32_t min, int32_t max)
    {
        powerBar.setMaxPower(max);
        powerScale.setRange(0, max);
    }

    void setProductionValue(int32_t solarProd, int32_t lowRatePower, int gridPower)
    {
        powerBar.setGridPower(gridPower);
        powerBar.setLowRatePower(lowRatePower);
        powerBar.setSolarPower(solarProd);
    }

    void setHomeConsumption(int32_t homeConsumption);

};
