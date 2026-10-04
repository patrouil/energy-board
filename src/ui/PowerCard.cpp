//
// Created by Vibe Code on 03/10/2026.
// // with the assistance of Mistral Vibe Code.

//

#include "Display.h"
#include "PowerCard.h"
#include "AppTask.h"
#include "Log.h"
#include "Theme.h"

static const lv_coord_t POWER_CARD_VALUE_OFFSET_X = -2;
static const lv_coord_t POWER_CARD_VALUE_OFFSET_Y = -10;

static const char * WATT_UNIT = "Wh";
static const char * KILOWATT_UNIT = "kWh";

PowerCard::~PowerCard()
{
    LOG_DEBUG("PowerCard destruction");
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
    lv_obj_set_style_text_font(valueLabel, Theme::EXTRA_LARGE_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(valueLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_label_set_text_static(valueLabel, this->valueBuffer);
    lv_obj_align(valueLabel, LV_ALIGN_TOP_MID, POWER_CARD_VALUE_OFFSET_X, POWER_CARD_VALUE_OFFSET_Y);

    unitLabel = lv_label_create(container);
    APP_ASSERT(unitLabel != nullptr);
    lv_obj_set_style_text_font(unitLabel, Theme::MEDIUM_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(unitLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_label_set_text_static(unitLabel, WATT_UNIT);
    lv_obj_align_to(unitLabel, valueLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);

    titleLabel = lv_label_create(container);
    APP_ASSERT(titleLabel != nullptr);
    lv_obj_set_style_text_font(titleLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(titleLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_label_set_text_static(titleLabel, titleBuffer);
    lv_obj_align(titleLabel, LV_ALIGN_BOTTOM_MID, 0, 0);

    updateValue();
    return container;
}

void PowerCard::setPower(int32_t watts)
{
    LOG_DEBUG("PowerCard::setPower value=%d", watts);
    this->watts = watts;
    updateValue();
}

void PowerCard::setTitle(const char* title)
{
    LOG_DEBUG("PowerCard::setTitle title=%s", title ? title : "(null)");
    if (titleLabel == nullptr) return;
    if (title == nullptr) return;
    snprintf(titleBuffer, sizeof(titleBuffer), "%s", title);
    //lv_label_set_text(titleLabel, titleBuffer);
}

void PowerCard::setBorderColor(lv_color_t color)
{
    LOG_DEBUG("PowerCard::setBorderColor");
    if (container == nullptr) return;
    lv_obj_set_style_border_color(container, color, LV_PART_MAIN);
}

void PowerCard::setBackgroundColor(lv_color_t color)
{
    LOG_DEBUG("PowerCard::setBackgroundColor");
    if (container == nullptr) return;
    lv_obj_set_style_bg_color(container, color, LV_PART_MAIN);
}

void PowerCard::updateValue()
{
    if ((valueLabel == nullptr) || (unitLabel == nullptr)) return;

    const char* unit = WATT_UNIT;
    int64_t absWatts = (watts < 0) ? -(int64_t)watts : watts;

    if (absWatts < 1000)
    {
        snprintf(valueBuffer, sizeof(valueBuffer), "%d", (int)watts);
        unit = WATT_UNIT;
    }
    else if (absWatts < 10000)
    {
        int64_t kilo = absWatts / 1000;
        int64_t centi = (absWatts % 1000) / 10;
        snprintf(valueBuffer, sizeof(valueBuffer), "%s%lld.%02lld",
                 (watts < 0) ? "-" : "", (long long)kilo, (long long)centi);
        unit = KILOWATT_UNIT;
    }
    else
    {
        int64_t kilo = absWatts / 1000;
        int64_t deci = (absWatts % 1000) / 100;
        snprintf(valueBuffer, sizeof(valueBuffer), "%s%lld.%lld",
                 (watts < 0) ? "-" : "", (long long)kilo, (long long)deci);
        unit = KILOWATT_UNIT;
    }

    //lv_label_set_text(valueLabel, valueBuffer);
    lv_label_set_text_static(unitLabel, unit);
    lv_obj_align_to(unitLabel, valueLabel, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
}
