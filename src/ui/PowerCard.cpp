//
// Created by Vibe Code on 03/10/2026.
// // with the assistance of Mistral Vibe Code.

//

#include "Display.h"
#include "PowerCard.h"
#include "AppTask.h"
#include "Log.h"
#include "Theme.h"
#include "../../include/wattHelper.h"

static const lv_coord_t POWER_CARD_VALUE_OFFSET_X = 0 ; // -2;
static const lv_coord_t POWER_CARD_VALUE_OFFSET_Y = 0; //-10;

PowerCard::~PowerCard()
{
    //LOG_DEBUG("PowerCard destruction");
    if (container) lv_obj_del(container);
    container = nullptr;
    valueLabel = nullptr;
    unitLabel = nullptr;
    titleLabel = nullptr;
}

lv_obj_t* PowerCard::create(lv_obj_t* parent)
{
    APP_ASSERT(parent != nullptr);
    container = lv_obj_create(parent);
    APP_ASSERT(container != nullptr);
    lv_obj_remove_style_all(container);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(container, CARD_WIDTH, CARD_HEIGHT);
    lv_obj_set_style_radius(container, CARD_RADIUS, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, CARD_BORDER_WIDTH, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, Theme::BLEU_EDF, LV_PART_MAIN);
    lv_obj_set_style_bg_color(container, Theme::PAGE_BACKGROUND_COLOR, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(container, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(container, 0, LV_PART_MAIN);

    valueLabel = lv_label_create(container);
    APP_ASSERT(valueLabel != nullptr);
    lv_obj_set_style_text_font(valueLabel, Theme::HUGE_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(valueLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_label_set_text_static(valueLabel, this->valueBuffer);
    lv_obj_align(valueLabel, LV_ALIGN_CENTER, 0, 0);

    unitLabel = lv_label_create(container);
    APP_ASSERT(unitLabel != nullptr);
    lv_obj_set_style_text_font(unitLabel, Theme::SMALL_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(unitLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_label_set_text_static(unitLabel, wattHelper::WATT_UNIT);
    lv_obj_align(unitLabel, LV_ALIGN_TOP_RIGHT, 0, 0);

    //lv_obj_align_to(unitLabel, valueLabel, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);

    titleLabel = lv_label_create(container);
    APP_ASSERT(titleLabel != nullptr);
    lv_obj_set_style_text_font(titleLabel, Theme::LARGE_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(titleLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_label_set_text_static(titleLabel, titleBuffer);
    lv_obj_align(titleLabel, LV_ALIGN_BOTTOM_MID, 0, 0);

    updateValue();
    return container;
}

void PowerCard::setPower(int32_t watts)
{
    //LOG_DEBUG("PowerCard::setPower value=%d", watts);
    this->watts = watts;
    updateValue();
}

void PowerCard::setTitle(const char* title)
{
   // LOG_DEBUG("PowerCard::setTitle title=%s", title ? title : "(null)");
    if (titleLabel == nullptr) return;
    if (title == nullptr) return;
    snprintf(titleBuffer, sizeof(titleBuffer), "%s", title);
    lv_label_set_text_static(titleLabel, titleBuffer);
}

void PowerCard::setBorderColor(lv_color_t color)
{
   // LOG_DEBUG("PowerCard::setBorderColor");
    if (container == nullptr) return;
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);
}

void PowerCard::setBackgroundColor(lv_color_t color)
{
   // LOG_DEBUG("PowerCard::setBackgroundColor");
    if (container == nullptr) return;
    lv_obj_set_style_bg_color(container, color, LV_PART_MAIN);
}

void PowerCard::updateValue()
{
  //  LOG_DEBUG("PowerCard::updateValue");

    if ((valueLabel == nullptr) || (unitLabel == nullptr)) return;

    const char* unit ;

    unit = wattHelper::labelOf(watts, valueBuffer, sizeof(valueBuffer));

    lv_label_set_text_static(valueLabel, valueBuffer);
    lv_label_set_text_static(unitLabel, unit);
}
