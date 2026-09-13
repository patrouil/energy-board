//
// Created by Patrick Rouillon on 12/09/2026.
//

#include "Display.h"
#include "PowerBar.h"
#include "AppTask.h"
#include "Log.h"
#include "Theme.h"

static const lv_coord_t POWER_BAR_HEIGHT = 30;

#define BLEU_EDF 0x2855C1
#define ROUGE_EDF 0xEB6332
#define BLANC_EDF 0xFFFFFF
#define VERT_EDF 0x57BF79


PowerBar::~PowerBar()
{
    LOG_DEBUG("PowerBar destruction");
    if (container) lv_obj_del(container);
    container = nullptr;
    background = nullptr;
    indicator = nullptr;
}

lv_obj_t* PowerBar::create(lv_obj_t* parent)
{
    APP_ASSERT(parent != nullptr);

    container = lv_obj_create(parent);
    APP_ASSERT(container != nullptr);
    lv_obj_remove_style_all(container);
    lv_obj_set_style_pad_all(container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    lv_coord_t parentWidth = lv_obj_get_content_width(parent);
    if (parentWidth <= 0) {
        parentWidth = lv_obj_get_width(parent);
    }
    if (parentWidth <= 0) {
        parentWidth = Display::displayWidth ;
    }
    lv_obj_set_size(container, parentWidth, POWER_BAR_HEIGHT);

    background = lv_bar_create(container);
    APP_ASSERT(background != nullptr);
    lv_obj_set_size(background, parentWidth, POWER_BAR_HEIGHT);
    lv_obj_align(background, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(background, lv_color_hex(BLANC_EDF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(background, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(background, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(background, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(background, 0, LV_PART_MAIN);

    indicator = lv_bar_create(container);
    APP_ASSERT(indicator != nullptr);
    lv_obj_set_height(indicator, POWER_BAR_HEIGHT);
    lv_obj_align(indicator, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(indicator, lv_color_hex(BLEU_EDF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(indicator, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(indicator, lv_color_hex(VERT_EDF), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(indicator, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(indicator, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(indicator, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(indicator, 0, LV_PART_MAIN);
    lv_bar_set_range(indicator, minPower, cheapPower);
    lv_bar_set_value(indicator, productionPower, LV_ANIM_OFF);

    updateIndicatorWidth();

    return container;
}

void PowerBar::setRange(int32_t min, int32_t max)
{
    this->minPower = min;
    this->maxPower = max;
    if (!indicator) return;
    LOG_DEBUG("PowerBar::setRange min=%d max=%d", min, max);
    lv_bar_set_range(indicator, min, max);
}

void PowerBar::setCheapValue(int32_t value)
{
    this->cheapPower = value;
    if (!indicator) return;
    // lv_bar_set_value(indicator, value, LV_ANIM_OFF);
    LOG_DEBUG("PowerBar::setCheapValue value=%d", value);
    this->updateIndicatorWidth();
}

void PowerBar::setProductionValue(int32_t value)
{
    this->productionPower = value;
    if (!indicator) return;
    LOG_DEBUG("PowerBar::setProductionValue value=%d", value);
    lv_bar_set_value(indicator, value, LV_ANIM_OFF);
}

/*
 *adjust width with power values and parent repaint
 */
void PowerBar::updateIndicatorWidth()
{
    if (!container || !indicator) return;
    lv_coord_t parentWidth = lv_obj_get_width(lv_obj_get_parent(this->container));
    lv_coord_t containerWidth = lv_obj_get_width(container);
    if ( parentWidth >= containerWidth-1)
    {
        LOG_DEBUG("PowerBar::updateIndicatorWidth parentWidth=%d containerWidth=%d", parentWidth, containerWidth);
        lv_obj_set_width(container, parentWidth-1);
         containerWidth = lv_obj_get_width(container);
        lv_obj_update_layout(container);
        lv_obj_invalidate(container);
    }

    if (containerWidth <= 0) return;

    ratioPercent = ( + cheapPower - minPower) * 100 / (maxPower - minPower) ;
    lv_coord_t indicatorWidth = (containerWidth) * ratioPercent / 100;
    LOG_DEBUG("PowerBar::updateIndicatorWidth min %d max %d cheap %d", minPower, maxPower, cheapPower);
    LOG_DEBUG("PowerBar::updateIndicatorWidth indicatorWidth=%d  ratio=%d", indicatorWidth, ratioPercent);
    if (indicatorWidth < 1) indicatorWidth = 1;
    lv_obj_set_width(indicator, indicatorWidth);
}
