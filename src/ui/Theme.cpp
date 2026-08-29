//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "Theme.h"


const lv_color_t Theme::BACKGROUND_COLOR = lv_color_hex(0x000000);
const lv_color_t Theme::TEXT_COLOR = lv_color_hex(0xFFFFFF);
const lv_color_t Theme::PAGE_BACKGROUND_COLOR = lv_color_hex(0x333333);
const lv_color_t Theme::BUTTON_COLOR = lv_color_hex(0x555555);
const lv_color_t Theme::PRIMARY_COLOR = lv_color_hex(0x003B77);
const lv_color_t Theme::SECONDARY_COLOR = lv_color_hex(0x0077B6);

const lv_font_t* Theme::DEFAULT_FONT = LV_FONT_MONTSERRAT_12;
const lv_font_t* Theme::MEDIUM_FONT = LV_FONT_MONTSERRAT_10;
const lv_font_t* Theme::SMALL_FONT = LV_FONT_MONTSERRAT_8;
const lv_font_t* Theme::LARGE_FONT = LV_FONT_MONTSERRAT_16;

const lv_coord_t Theme::LABEL_SPACING = 20;

Theme::Theme()
    : primaryColor(Theme::PRIMARY_COLOR),
      secondaryColor(Theme::SECONDARY_COLOR),
      backgroundColor(Theme::PAGE_BACKGROUND_COLOR),
      textColor(Theme::TEXT_COLOR),
      font((Theme::DEFAULT_FONT)) {

    lv_style_init(&style);
}


Theme::~Theme() {
    if (theme) {
        // lv_theme_delete(theme);
    }
}


Theme& Theme::getInstance() {
    static Theme instance;
    return instance;
}


void Theme::init(lv_disp_t *disp) {
    lv_style_set_bg_color(&style, backgroundColor);
    lv_style_set_text_color(&style, textColor);
    lv_style_set_text_font(&style, font);

    static lv_theme_t* theme_ptr = lv_theme_default_init(disp,
        primaryColor, secondaryColor,
        LV_THEME_DEFAULT_DARK, font);
    theme = theme_ptr;
}

void Theme::apply(lv_obj_t* obj) {
    lv_obj_set_style_bg_color(obj, PAGE_BACKGROUND_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_color(obj, TEXT_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_font(obj, DEFAULT_FONT, LV_PART_MAIN);
}


lv_theme_t* Theme::getTheme() const {
    return theme;
}