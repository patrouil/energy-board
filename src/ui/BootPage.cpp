//
// Created by Patrick Rouillon on 03/09/2026.
//

#include "BootPage.h"

void BootPage::create()
{
    Screen::create();

    lv_coord_t width = lv_disp_get_hor_res(display.lvgl_display());
    lv_coord_t height = lv_disp_get_ver_res(display.lvgl_display());
    lv_obj_set_size(page, width, height);
    lv_obj_t* label = lv_label_create(page);
    obj_label = label;
    lv_label_set_text(obj_label, "boot ....");
    lv_obj_align(obj_label, LV_ALIGN_CENTER, 0, 0);

}

 BootPage::~BootPage()
{
    if ( obj_label) lv_obj_del(obj_label);
    obj_label = nullptr;
}

void BootPage::create(unPhone& phone)
{
    String LVGL_Arduino = "Loading Energy Monitor! ";

    this->create();
    String v = "Hardware version " + String(phone.version());
    LVGL_Arduino.concat(v);
    lv_label_set_text(obj_label, LVGL_Arduino.c_str());
}
