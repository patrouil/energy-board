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

    static const lv_color_t PRIMARY_COLOR;
    static const lv_color_t PAGE_BACKGROUND_COLOR;
    static const lv_color_t SECONDARY_COLOR;

    static const lv_coord_t LABEL_SPACING;

    Theme();
    ~Theme();
    void init(lv_disp_t *);

    void apply(lv_obj_t* obj);

    lv_theme_t* getTheme() const;
    // Supprime les constructeurs publics
    Theme(Theme const&) = delete;
    void operator=(Theme const&) = delete;

private:
    static Theme* instance;
    lv_theme_t* theme;

    lv_style_t style;
    lv_color_t primaryColor;
    lv_color_t secondaryColor;
    lv_color_t backgroundColor;
    lv_color_t textColor;
    const lv_font_t* font;

};



#endif //ENERGY_BOARD_THEME_H