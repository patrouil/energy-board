//
// Created by Patrick Rouillon on 12/09/2026.
//

#pragma once

#ifndef ENERGY_BOARD_POWERBAR_H
#define ENERGY_BOARD_POWERBAR_H

#include <lvgl.h>

/*
 * PowerBar : widget composite.
 *
 *   - barre de fond horizontale, 20px de haut, blanche, largeur = largeur du parent.
 *   - barre d'indicateur posee dessus, 20px de haut, fond bleu, indicateur vert.
 *     Sa largeur est proportionnelle a celle du parent (70% par defaut).
 *     La largeur de l'indicateur (valeur) est variable.
 *
 * Min/Max de l'indicateur sont variables (setRange). Valeur via setValue.
 * La proportion (largeur de la barre d'indicateur / largeur du parent) est
 * reglable via setRatio (en pourcentage de la largeur du parent).
 */
class PowerBar
{
public:
    PowerBar() = default;
    ~PowerBar();

    lv_obj_t* create(lv_obj_t* parent);

    void setRange(int32_t min, int32_t max);
    void setCheapValue(int32_t value);
    void setProductionValue(int32_t value);

    lv_obj_t* getContainer() const { return container; }
    lv_obj_t* getBackground() const { return background; }
    lv_obj_t* getIndicator() const { return indicator; }

private:
    lv_obj_t* container = nullptr;
    lv_obj_t* background = nullptr;
    lv_obj_t* indicator = nullptr;
    uint32_t ratioPercent = 70;

    void updateIndicatorWidth();

    int32_t minPower = 0;
    int32_t maxPower = 100;
    int32_t productionPower = 0;
    int32_t cheapPower = 0;

};

#endif // ENERGY_BOARD_POWERBAR_H
