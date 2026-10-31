//
// Created by Patrick Rouillon on 04/02/2026.
//

#include <Log.h>

#include "Display.h"
#include "Theme.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t color_buffer[Display::displayWidth * Display::displayHeight / 10];

lv_disp_drv_t Display::disp_drv;

Display::Display() : _tft_screen(displayWidth, displayHeight)
{
}

Display::~Display()
{
    if (this->lv_timer)
        lv_timer_del(this->lv_timer);
    this->lv_timer = nullptr;
    // TODO unregister driver
    this->disp = nullptr;
}

// call it once.
void Display::init()
{
    // LOG_DEBUG("Display::init: %s", "init tft");
    _tft_screen.init();
    _tft_screen.setRotation(1); /* Landscape orientation */
    /*Set the touchscreen calibration data,
     the actual data for your display can be acquired using
     the Generic -> Touch_calibrate example from the TFT_eSPI library*/
    uint16_t calData[5] = {347, 3549, 419, 3352, 5};
    _tft_screen.setTouch(calData);
    //   LOG_DEBUG("Display::init %s", "init lv");
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, color_buffer, nullptr, sizeof(color_buffer) / sizeof(color_buffer[0]));
    // prepare driver
    //   LOG_DEBUG("Display::init %s", "driver");
    lv_disp_drv_init(&(disp_drv));
    disp_drv.hor_res = displayWidth;
    disp_drv.ver_res = displayHeight;
    disp_drv.flush_cb = Display::display_flush;
    disp_drv.draw_buf = &draw_buf;
    this->disp = lv_disp_drv_register(&disp_drv);
    lv_disp_set_default(this->disp);
}

void Display::setOrientation(lv_disp_rot_t rotation)
{
    lv_disp_set_rotation(disp, rotation);
}

/* Display flushing */
void Display::display_flush(
    lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p
)
{
    // LOG_DEBUG("Display::display_flush %x", disp);
    int32_t w = (area->x2 - area->x1 + 1);
    int32_t h = area->y2 - area->y1 + 1;
    TFT_eSPI& tft = Display::getInstance().get_tft_screen();

    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}
