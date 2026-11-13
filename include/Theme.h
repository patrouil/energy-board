//
// Created by Patrick Rouillon on 04/02/2026.
//

#ifndef ENERGY_BOARD_THEME_H
#define ENERGY_BOARD_THEME_H

#include <lvgl.h>

class Theme {
public:
    static Theme& getInstance();

    static const lv_color_t BACKGROUND_COLOR;
    static const lv_color_t TEXT_COLOR;
    static const lv_color_t BUTTON_COLOR;
    static const lv_font_t* DEFAULT_FONT;

    static const lv_font_t* MEDIUM_FONT;
    static const lv_font_t* SMALL_FONT;
    static const lv_font_t* LARGE_FONT;
    static const lv_font_t* HUGE_FONT;

    static const lv_color_t PRIMARY_COLOR;
    static const lv_color_t PAGE_BACKGROUND_COLOR;
    static const lv_color_t SECONDARY_COLOR;

    static const lv_coord_t LABEL_SPACING;

    static const lv_color_t BLEU_EDF;
    static const lv_color_t ROUGE_EDF;
    static const lv_color_t BLANC_EDF;
    static const lv_color_t VERT_EDF;

    void default_init(lv_disp_t *);

    void apply(lv_obj_t* obj);


private:
    Theme();

    lv_style_t style;
    lv_color_t primaryColor;
    lv_color_t secondaryColor;
    lv_color_t backgroundColor;
    lv_color_t textColor;
    const lv_font_t* font;

};



#endif //ENERGY_BOARD_THEME_H