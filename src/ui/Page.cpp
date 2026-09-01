//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "../../include/Page.h"

#include "AppTask.h"
#include "Display.h"
#include "Theme.h"


Page::Page(Display& parent) : display(parent)
{
    this->page = lv_obj_create(Display::me->getActiveScreen());
    if (!this->page) {  // ✅ Vérifier avant d'utiliser
        LOG_ERROR("Page::Page : lv_obj_create failed!");
        return;
    }
    lv_coord_t width = lv_disp_get_hor_res(display.lvgl_display());
    lv_coord_t height = lv_disp_get_ver_res(display.lvgl_display());
    lv_obj_set_size(this->page, width , height );
    // lv_obj_align(this->page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(this->page, Theme::BACKGROUND_COLOR, LV_PART_MAIN);

    // Theme::getInstance().apply(page);
    APP_ASSERT(lv_obj_is_valid(this->page));
    LOG_DEBUG("Page::Page : construction");
}

Page::~Page()
{
}

void Page::show()
{
    lv_scr_load(this->page);
    //lv_obj_set_style_bg_opa(page, LV_OPA_100, LV_PART_MAIN);
    //lv_obj_clear_flag(page, LV_OBJ_FLAG_HIDDEN);
}

void Page::hide()
{
    lv_obj_add_flag(page, LV_OBJ_FLAG_HIDDEN);
}
