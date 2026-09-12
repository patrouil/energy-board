//
// Created by Patrick Rouillon on 12/09/2026.
//

#include "PowerBar.h"
#include "AppTask.h"
#include "Log.h"
#include "Theme.h"

static const lv_coord_t POWER_BAR_HEIGHT = 20;

PowerBar::~PowerBar()
{
    LOG_DEBUG("PowerBar destruction");
    if (container) lv_obj_del(container);
    container = nullptr;
    background = nullptr;
    indicator = nullptr;
}

void PowerBar::create(lv_obj_t* parent)
{
    LOG_DEBUG("PowerBar::create");
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
        parentWidth = 100;
    }
    lv_obj_set_size(container, parentWidth, POWER_BAR_HEIGHT);

    background = lv_bar_create(container);
    APP_ASSERT(background != nullptr);
    lv_obj_set_size(background, parentWidth, POWER_BAR_HEIGHT);
    lv_obj_align(background, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(background, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(background, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(background, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(background, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(background, 0, LV_PART_MAIN);

    indicator = lv_bar_create(container);
    APP_ASSERT(indicator != nullptr);
    lv_obj_set_height(indicator, POWER_BAR_HEIGHT);
    lv_obj_align(indicator, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(indicator, lv_color_hex(0x0077B6), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(indicator, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(indicator, lv_color_hex(0x00C853), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(indicator, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(indicator, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(indicator, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(indicator, 0, LV_PART_MAIN);
    lv_bar_set_range(indicator, 0, 100);
    lv_bar_set_value(indicator, 0, LV_ANIM_OFF);

    updateIndicatorWidth();

    LOG_DEBUG("PowerBar::create done");
}

void PowerBar::setRange(int32_t min, int32_t max)
{
    if (!indicator) return;
    lv_bar_set_range(indicator, min, max);
}

void PowerBar::setValue(int32_t value)
{
    if (!indicator) return;
    lv_bar_set_value(indicator, value, LV_ANIM_OFF);
}

void PowerBar::setRatio(uint8_t percent)
{
    if (percent > 100) percent = 100;
    ratioPercent = percent;
    updateIndicatorWidth();
}

void PowerBar::updateIndicatorWidth()
{
    if (!container || !indicator) return;
    lv_coord_t containerWidth = lv_obj_get_width(container);
    if (containerWidth <= 0) return;
    lv_coord_t indicatorWidth = static_cast<lv_coord_t>(
        (static_cast<int32_t>(containerWidth) * ratioPercent) / 100);
    if (indicatorWidth < 1) indicatorWidth = 1;
    lv_obj_set_width(indicator, indicatorWidth);
}
