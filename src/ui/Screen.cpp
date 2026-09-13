//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "Screen.h"

#include "AppTask.h"
#include "Display.h"
#include "Theme.h"



Screen::~Screen()
{
    if ( this->page) lv_obj_del(this->page);
    this->page = nullptr;
}

void Screen::create()
{
    this->page = lv_obj_create(lv_scr_act());  // should be nullptf
    if (!this->page) {  // ✅ Vérifier avant d'utiliser
        LOG_ERROR("Page::Page : lv_obj_create failed!");
        return;
    }
    Display & display = Display::getInstance();
    lv_coord_t width = lv_disp_get_hor_res(display.lvgl_display());
    lv_coord_t height = lv_disp_get_ver_res(display.lvgl_display());
  //  LOG_DEBUG("Page::Page : resolution %d x %d", width, height);

    lv_obj_set_size(this->page, width , height );
    // lv_obj_align(this->page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(this->page, Theme::PAGE_BACKGROUND_COLOR, LV_PART_MAIN);
    //lv_obj_set_style_bg_color(page, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN); // Bleu (palette)

    APP_ASSERT(lv_obj_is_valid(this->page));
 //   LOG_DEBUG("Page::Page : construction");
}

void Screen::show()
{
 //   LOG_DEBUG("Screen::show : ");

    //lv_scr_load(this->page);
    //lv_obj_set_style_bg_opa(page, LV_OPA_100, LV_PART_MAIN);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(this->page);
}

void Screen::hide()
{
    lv_obj_add_flag(this->page, LV_OBJ_FLAG_HIDDEN);
}
