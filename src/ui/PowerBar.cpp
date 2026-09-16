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

#define POWER_BAR_MIN_POWER 0

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
    if (parentWidth <= 0)
    {
        parentWidth = lv_obj_get_width(parent);
    }
    if (parentWidth <= 0)
    {
        parentWidth = Display::displayWidth;
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
    lv_obj_set_size(indicator, ratioPercent * parentWidth / 100, POWER_BAR_HEIGHT);  // initial value
    lv_obj_align(indicator, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(indicator, lv_color_hex(BLEU_EDF), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(indicator, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_set_style_bg_color(indicator, lv_color_hex(VERT_EDF), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(indicator, LV_OPA_COVER, LV_PART_INDICATOR);

    lv_obj_set_style_border_width(indicator, 2, LV_PART_MAIN);
    lv_obj_set_style_radius(indicator, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(indicator, 0, LV_PART_MAIN);

    lv_bar_set_range(indicator, POWER_BAR_MIN_POWER, lowRatePower);
    lv_bar_set_value(indicator, solarPower, LV_ANIM_OFF);

    updateBarSize();

    return container;
}

void PowerBar::setLowRatePower(int32_t value)
{
    this->lowRatePower = value;
    if (!indicator) return;
    LOG_DEBUG("PowerBar::setLowRatePower value=%d", value);

    if ( lowRatePower  <= solarPower )
        lv_obj_add_flag(indicator, LV_OBJ_FLAG_HIDDEN); // masquer
    else
    {
        lv_obj_clear_flag(indicator, LV_OBJ_FLAG_HIDDEN); // afficher
        lv_bar_set_range(indicator, POWER_BAR_MIN_POWER, lowRatePower);
        this->updateBarSize();
    }
}

void PowerBar::setSolarPower(int32_t value)
{
    LOG_DEBUG("PowerBar::setSolarPower value=%d", value);

    if ( solarPower > 0 ) this->solarPower = value;
    calculatePowerUsage();
    if (!indicator) return;
    lv_bar_set_value(indicator, this->solarPower, LV_ANIM_OFF);
}

void PowerBar::setGridPower(int32_t value)
{
    LOG_DEBUG("PowerBar::setGridPower value=%d", value);

    this->gridPower = value;
}

void PowerBar::setMaxPower(int32_t value)
{
    if ( maxPower > 0 ) this->maxPower = value;
    this->updateBarSize();
}

void PowerBar::setHomePowerUsage(int32_t value)
{
    this->homePowerUsage = value;
}

/*
 *adjust width with power values and parent repaint
 */
void PowerBar::updateBarSize()
{
    if (!container || !indicator) return;
    lv_coord_t containerWidth = lv_obj_get_width(container);
    if (containerWidth <= 0) return;  // savefy at startup.
    // loaw rate bar size is a pencentage of lowRatePower vs maxPower
    ratioPercent = lowRatePower * 100 / maxPower;
    lv_coord_t indicatorWidth = (containerWidth) * ratioPercent / 100;
    LOG_DEBUG("PowerBar::updateBarSize low rate %d max power %d", lowRatePower, maxPower);
    LOG_DEBUG("PowerBar::updateBarSize indicatorWidth=%d  ratio=%d", indicatorWidth, ratioPercent);

    lv_obj_set_width(indicator, indicatorWidth);
    //lv_bar_set_value(indicator, this->lowRatePower/2, LV_ANIM_OFF);
   //  lv_obj_invalidate(container);

}
