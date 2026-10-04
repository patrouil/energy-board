//
// Created by Vibe Code on 16/09/2026.
// with the assistance of Mistral Vibe Code.
//
#pragma once

#include <freertos/FreeRTOS.h>
#include <semphr.h>
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
    void setHomeConsumption(int32_t homeConsumption);
    void setHomeConsumptionColor(lv_color_t color);

    lv_obj_t* getContainer() const { return container; }

private:
    static const uint16_t MAX_TICKS = 36;
    static const uint16_t MAX_LABEL = 8;

    SemaphoreHandle_t logMutex = xSemaphoreCreateMutex();

    lv_obj_t* container = nullptr;
    lv_obj_t* axisLine = nullptr;

    int32_t homeConsumption = -1;
    lv_obj_t* homeConsumptionLabel = nullptr;
    char homeConsumptionLabelValue[MAX_LABEL] = {'\0'};


    int32_t minPower = 0;
    int32_t maxPower = 1000;
    int32_t tickInterval = 500; // Wh
    uint16_t majorTickEvery = 2;  // 1000 Wh
    bool labelShow = false;

    lv_obj_t* tickLines[MAX_TICKS] = {nullptr};
    lv_obj_t* tickLabels[MAX_TICKS] = {nullptr};
    char tickLabelsValues[MAX_TICKS][MAX_LABEL]; // statically allocated labeld.
    int32_t tickValues[MAX_TICKS] = {0};
    uint16_t tickCount = 0;

    lv_point_t axisPoints[2];
    // 2 points per line.
    lv_point_t tickPoints[MAX_TICKS * 2];

    void rebuild();
    lv_coord_t valueToX(int32_t value, lv_coord_t width) const;
    void destroyTicks();
    void updateHomeConsumptionPos();
};
