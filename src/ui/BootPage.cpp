//
// Created by Patrick Rouillon on 03/09/2026.
//

#include "BootPage.h"
#include "Log.h"
#include "Theme.h"

/*
*lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_PART_MAIN); // Noir
lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), LV_PART_MAIN); // Blanc
lv_obj_set_style_bg_color(lv_scr_act(), lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN); // Bleu (palette)
*/

void BootPage::create()
{
    Screen::create();

    //lv_coord_t width = lv_disp_get_hor_res(display.lvgl_display());
    //lv_coord_t height = lv_disp_get_ver_res(display.lvgl_display());

    //lv_obj_set_size(page, width, height);
    lv_obj_t* label = lv_label_create(page);
    obj_label = label;
    lv_label_set_text(obj_label, "boot ....");
    lv_obj_set_style_text_color(obj_label, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_align(obj_label, LV_ALIGN_CENTER, 0, 0);

}

 BootPage::~BootPage()
{
    LOG_DEBUG("Boot Page destruction");
    if ( obj_label) lv_obj_del(obj_label);
    obj_label = nullptr;
}

/*
 * Warning : there is a TOUCH_CS declaration collition.
 * unphone declares uint TOUCH_CS = 38
 * and TFT_SPI User_Setup declare #define TOUCH_CS 38
 * NEVER load both includes.
 */
void BootPage::set_version(uint8_t  vers)
{
    LOG_DEBUG("Boot Page creation");
    String LVGL_Arduino = "Loading Energy Monitor! ";

    this->create();
    String v = "Hardware version " + String(vers);
    LVGL_Arduino.concat(v);
    lv_label_set_text(obj_label, LVGL_Arduino.c_str());
    lv_obj_set_style_text_color(obj_label, lv_color_hex(0xFF0000), LV_PART_MAIN);

}
