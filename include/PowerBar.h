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

    void create(lv_obj_t* parent);

    void setRange(int32_t min, int32_t max);
    void setValue(int32_t value);
    void setRatio(uint8_t percent);

    lv_obj_t* getContainer() const { return container; }
    lv_obj_t* getBackground() const { return background; }
    lv_obj_t* getIndicator() const { return indicator; }

private:
    lv_obj_t* container = nullptr;
    lv_obj_t* background = nullptr;
    lv_obj_t* indicator = nullptr;
    uint8_t ratioPercent = 70;

    void updateIndicatorWidth();
};

#endif // ENERGY_BOARD_POWERBAR_H
