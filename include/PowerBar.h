//
// Created by Patrick Rouillon on 12/09/2026.
//

#pragma once

#include <lvgl.h>

/*
 * PowerBar : widget composite.
 *
 *   - barre de fond horizontale, 20px de haut, blanche, largeur = largeur du parent.
 *   - barre d'indicateur posee dessus, 20px de haut, fond bleu, indicateur vert.
 *     Sa largeur est proportionnelle a celle du parent (70% par defaut).
 *     La largeur de l'indicateur (valeur) est variable.
 *
 * The bar display is divided in three parts : free solar power, low rate power and normal rate power.
 * solar power is your solar panel production. The energy is assumed as been free.
 * low rate power is an arbitraty value. For example it can be solar production X2.
 * max energy is the maximum value of the bar. This can be your grid limit for example. But for a nicer look way is to use a multiple of low rate.
 * home consumption is calculated automatically with the rule consuption = grid usage - solar production.
 *
 * Here are somes rules :
 *  solar power is betwwen 0 and P. The unit is watts.
 *  low rate power is between solar power and X.  If low rate os zero, or below solar power value, both bars are hiden.
 *  grid power is a value between minus solar power (if energy is exported) and max power.
 *  If grid usage is above max energy, the bar is full.
 *
 *
 */
class PowerBar
{
public:
    PowerBar() = default;
    ~PowerBar();

    lv_obj_t* create(lv_obj_t* parent);

    void setLowRatePower(int32_t value);
    void setSolarPower(int32_t value);
    void setGridPower(int32_t value);
    void setMaxPower(int32_t value);
    void setHomePowerUsage(int32_t value);

    lv_obj_t* getContainer() const { return container; }


private:
    lv_obj_t* container = nullptr;
    lv_obj_t* background = nullptr;
    lv_obj_t* indicator = nullptr;
    uint32_t ratioPercent = 70;

    lv_obj_t* getBackground() const { return background; }
    lv_obj_t* getIndicator() const { return indicator; }

    void calculatePowerUsage();
    void updateBarSize();

    int32_t gridPower = 0;
    int32_t maxPower = 100;
    int32_t solarPower = 10;
    int32_t lowRatePower = 20;
    int32_t homePowerUsage = 0;


};
