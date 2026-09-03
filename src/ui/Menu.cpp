//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "../../include/Menu.h"
#include "Theme.h"

Menu::Menu(lv_obj_t* parent) {
    menuContainer = lv_obj_create(parent);
    Theme::getInstance().apply(menuContainer);
}

void Menu::addPage(Screen* page, const char* name) {
    windows.push_back(page);
    lv_obj_t* btn = lv_btn_create(menuContainer);
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, name);
    lv_obj_add_event_cb(btn, [](lv_event_t* e) {
        lv_obj_t* btn = lv_event_get_target(e);
        uint32_t index = lv_btnmatrix_get_selected_btn(btn);
        // Logique pour afficher la fenêtre correspondante
    }, LV_EVENT_CLICKED, nullptr);
}

void Menu::show() {
    lv_obj_clear_flag(menuContainer, LV_OBJ_FLAG_HIDDEN);
}
