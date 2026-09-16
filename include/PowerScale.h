//
// Created by Vibe Code on 16/09/2026.
//
#pragma once

#include <lvgl.h>

/*
 * PowerScale : widget composite imitant le widget lv_scale de LVGL.
 *
 *   - echelle horizontale (mode similaire a LV_SCALE_MODE_HORIZONTAL_BOTTOM).
 *   - un tick mineur tous les "tickInterval" watts (500 W par defaut).
 *   - un tick majeur (plus long, avec etiquette) tous les "majorTickEvery"
 *     ticks (2 par defaut, soit 1000 W).
 *   - une ligne d'axe traversant le haut du widget.
 *
 * La plage [min, max] est exprimee en watts. Les positions des ticks sont
 * proportionnelles a la largeur du conteneur parent.
 */
class PowerScale
{
public:
    PowerScale() = default;
    ~PowerScale();

    lv_obj_t* create(lv_obj_t* parent);

    void setRange(int32_t min, int32_t max);
    void setTickInterval(int32_t intervalWatts);
    void setMajorTickEvery(uint16_t tickCount);
    void setLabelShow(bool show);

    lv_obj_t* getContainer() const { return container; }

private:
    static const uint16_t MAX_TICKS = 64;

    lv_obj_t* container = nullptr;
    lv_obj_t* axisLine = nullptr;

    int32_t minPower = 0;
    int32_t maxPower = 100;
    int32_t tickInterval = 500;
    uint16_t majorTickEvery = 2;
    bool labelShow = true;

    lv_obj_t* tickLines[MAX_TICKS] = {nullptr};
    lv_obj_t* tickLabels[MAX_TICKS] = {nullptr};
    int32_t tickValues[MAX_TICKS] = {0};
    uint16_t tickCount = 0;

    lv_point_t axisPoints[2];
    lv_point_t tickPoints[MAX_TICKS * 2];

    void rebuild();
    lv_coord_t valueToX(int32_t value, lv_coord_t width) const;
    void destroyTicks();
};
