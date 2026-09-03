//
// Created by Patrick Rouillon on 04/02/2026.
//

#ifndef ENERGY_BOARD_SCREENMANAGER_H
#define ENERGY_BOARD_SCREENMANAGER_H

#include <lvgl.h>
#include <TFT_eSPI.h>

/*Change to your screen resolution*/
static constexpr uint16_t displayWidth = 480;
static constexpr uint16_t displayHeight = 320;

class Display
{
public:
    static Display* me;

    Display();

    void init();
    void setBackgroundColor(lv_color_t color);
    void setOrientation(lv_disp_rot_t rotation);

    lv_obj_t* getActiveScreen() const
    {
        return lv_scr_act();
    }

    TFT_eSPI& get_tft_screen()
    {
        return _tft_screen;
    }

    lv_disp_t* lvgl_display() const
    {
        return disp;
    }

private:
     TFT_eSPI _tft_screen;
    static lv_disp_drv_t disp_drv;
    lv_disp_t* disp = nullptr;

    static void display_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p);
};

#endif //ENERGY_BOARD_SCREENMANAGER_H
