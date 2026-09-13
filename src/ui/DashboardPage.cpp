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
    if (bottomMessage) lv_obj_del(bottomMessage);
    oneLabel = nullptr;
    bottomMessage = nullptr;
}

lv_obj_t* DashboardPage::create_ticker(lv_obj_t* parent)
{
    lv_obj_t* ticker = lv_label_create(parent);

    /* The label must be narrower than the text */
    //lv_obj_set_width(ticker, 220);
    // lv_obj_set_height(ticker, LV_SIZE_CONTENT);
    lv_label_set_text(
        ticker,
        "  Welcome to my device  •  Temperature: 24°C  •  System ready  "
    );
    /* Continuous horizontal scrolling */
    lv_label_set_long_mode(
        ticker,
        LV_LABEL_LONG_SCROLL_CIRCULAR
    );
    /* Optional appearance */
    lv_obj_set_style_text_color(
        ticker,
        lv_color_hex(0xFFFFFF),
        LV_PART_MAIN
    );
    lv_obj_set_style_bg_color(
        ticker,
        lv_color_hex(0x202020),
        LV_PART_MAIN
    );

    lv_obj_set_style_bg_opa(
        ticker,
        LV_OPA_COVER,
        LV_PART_MAIN
    );
    lv_obj_set_style_pad_all(ticker, 8, LV_PART_MAIN);

    lv_obj_center(ticker);
    return ticker;
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
/*
    oneLabel = lv_label_create(this->page);
    APP_ASSERT(oneLabel != nullptr);
    lv_label_set_text(oneLabel, "One");
    lv_obj_set_style_text_color(oneLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_set_grid_cell(oneLabel,
                         LV_GRID_ALIGN_START, 0, 1,
                         LV_GRID_ALIGN_START, 0, 1);
*/
    lv_obj_t * b = powerBar.create(this->page);
    APP_ASSERT(b != nullptr);

    lv_obj_set_grid_cell(b,
                       LV_GRID_ALIGN_START, 0, 1,
                       LV_GRID_ALIGN_START, 0, 1);

    bottomMessage = create_ticker(this->page);
    APP_ASSERT(bottomMessage != nullptr);
    lv_obj_set_grid_cell(bottomMessage,
                         LV_GRID_ALIGN_START, 0, 1,
                         LV_GRID_ALIGN_START, 2, 1);

    LOG_DEBUG("DashboardPage::create done");
}

void DashboardPage::setBottomMessage(const char* message)
{
    lv_label_set_text(bottomMessage, message);
}