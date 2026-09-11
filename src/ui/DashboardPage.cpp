//
// Created by Patrick Rouillon on 11/09/2026.
//

#include "DashboardPage.h"

#include "AppTask.h"
#include "Log.h"
#include "Theme.h"

static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static const lv_coord_t row_dsc[] = {LV_GRID_CONTENT, LV_GRID_FR(1), LV_GRID_CONTENT, LV_GRID_TEMPLATE_LAST};

DashboardPage::~DashboardPage()
{
    LOG_DEBUG("DashboardPage destruction");
    if (oneLabel) lv_obj_del(oneLabel);
    if (lastLabel) lv_obj_del(lastLabel);
    oneLabel = nullptr;
    lastLabel = nullptr;
}

void DashboardPage::create()
{
    LOG_DEBUG("DashboardPage::create");
    Screen::create();

    lv_obj_set_layout(this->page, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(this->page, col_dsc, row_dsc);
    lv_obj_set_style_pad_all(this->page, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(this->page, 0, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(this->page, LV_SCROLLBAR_MODE_OFF);

    oneLabel = lv_label_create(this->page);
    APP_ASSERT(oneLabel != nullptr);
    lv_label_set_text(oneLabel, "One");
    lv_obj_set_style_text_color(oneLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_set_grid_cell(oneLabel,
        LV_GRID_ALIGN_START, 0, 1,
        LV_GRID_ALIGN_START, 0, 1);

    lastLabel = lv_label_create(this->page);
    APP_ASSERT(lastLabel != nullptr);
    lv_label_set_text(lastLabel, "Last");
    lv_obj_set_style_text_color(lastLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_set_grid_cell(lastLabel,
        LV_GRID_ALIGN_START, 0, 1,
        LV_GRID_ALIGN_START, 2, 1);

    LOG_DEBUG("DashboardPage::create done");
}
