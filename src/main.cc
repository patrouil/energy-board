//
// Created by Patrick Rouillon on 04/01/2026.
//

// sketch.ino ////////////////////////////////////////////////////////////////
// LVGL on unPhone demo //////////////////////////////////////////////////////
//
// derived from
// https://github.com/lvgl/lvgl/blob/master/examples/arduino/LVGL_Arduino/LVGL_Arduino.ino
// see also https://docs.lvgl.io/master/get-started/platforms/arduino.html

// we use the touchscreen (XPT2046) driver from the unPhone library (with the
// TFT driver from TFT_eSPI)


#include <unPhone.h>

#include <mqtt_client.h>

//#include <Adafruit_SPIFlash.h> // for LDF
#include <Log.h>
#define LV_USE_LOG 1
#include <lvgl.h>                       // LVGL //////////////////////////////
#define CONFIG_IDF_TARGET_ESP32S3 1

#include <TFT_eSPI.h>
#include <AppWifiControler.h>
#include <Display.h>
#include "AppMainControler.h"
#include "PageRouter.h"
#include "Theme.h"
#include "BootPage.h"

// create an unPhone; add a custom version of Arduino's map command for
// translating from touchscreen coordinates to LCD coordinates

//long my_mapper(long, long, long, long, long);
void my_print(const char* buf)
{
    Serial.printf(buf);
    Serial.flush();
}


// map touch coords to lcd coords
// a version of map that never returns out of range values
long my_mapper(long x, long in_min, long in_max, long out_min, long out_max)
{
    long probable =
        (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    if (probable < out_min) return out_min;
    if (probable > out_max) return out_max;
    return probable;
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data)
{
    uint16_t touchX, touchY;
    unPhone * this_phone_u = unPhone::me;

    // start of changes for unPhone ////////////////////////////////////////////
    bool touched = this_phone_u->tsp->touched();

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        TS_Point p(-1, -1, -1);
        p = this_phone_u->tsp->getPoint();

        // filter the ghosting on version 9 boards (on USB power; ~300 pressure)
#if UNPHONE_SPIN >= 9
        if (p.z < 400) return;
        //  D("probable ghost reject @ p.x(%04d), p.y(%04d) p.z(%04d)\n", p.x,p.y,p.z)
#endif

        Serial.printf("   p.x(%04d),  p.y(%04d) p.z(%04d)\n", p.x, p.y, p.z);
        if (p.x < 0 || p.y < 0)
            D("************* less than zero! *************\n")

        long xMin = 320;
        long xMax = 3945;
        long yMin = 420;
        long yMax = 3915;

        long xscld = my_mapper((long)p.x, xMin, xMax, 0, (long)Display::displayWidth);
        long yscld = // Y is inverted on rotation 1 (landscape, buttons right)
            static_cast<long>(Display::displayHeight) -
            my_mapper((long)p.y, yMin, yMax, 0, (long)Display::displayHeight);
        touchX = (uint16_t)xscld;
        touchY = (uint16_t)yscld;

        Serial.printf("touchX(%4d), touchY(%4d)\n", touchX, touchY);
        // end of changes for unPhone ////////////////////////////////////////////

        data->point.x = touchX;
        data->point.y = touchY;
        Serial.printf("Data x %u, Data y %u\n", touchX, touchY);
    }
}

void panic_handler(void* arg)
{
    printf("PANIC: %s\n", (char*)arg);
    // Logs ou actions avant redémarrage
    delay(100000);

}

void launch_tasks()
{

    AppMainControler& this_controler = AppMainControler::getInstance();

    this_controler.start();
    this_controler.get_wifi_controler()->start();
    this_controler.get_unphone_controler()->start();
    // this_controler.get_mqtt_controler()->start();
    this_controler.get_simulation_controler()->start();
}

void setup()
{
    Display& dsp = Display::getInstance();
    PageRouter &router = PageRouter::getInstance();
    unPhone ph = unPhone(); // create then access over global.

    LOG_INIT(Log::DEBUG, 115200);
    LOG_DEBUG("unphone  init");

    ph.begin();

    ph.expanderPower(false);
    ph.backlight(true);
    // after unPhone init
    LOG_DEBUG("display  init");

    try
    {
        dsp.init();
        Theme::getInstance().init(dsp.lvgl_display());
        ph.tftp = (void*)&(dsp.get_tft_screen());

        ph.tsp->setRotation(1);
        ph.backlight(true);

    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Exception standard capturée : %s", e.what());
    }
    catch (...)
    {
        LOG_ERROR("Une exception inconnue s'est produite.");
    }

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

     BootPage *boot_page = static_cast<BootPage*>(router.get_screen(ScreenId::BOOT_PAGE));
    boot_page->set_version(ph.version());
    boot_page->show();
    lv_timer_handler();
    sleep(3);
    //LOG_DEBUG("let start config");
    // first of all load config
    AppConfig &this_config = AppConfig::getInstance();
    this_config.loadDefaults(); // start by defaults values
    this_config.loadConfigPerf(); // then override

    //LOG_DEBUG("setup : create tasks");

    AppMainControler& this_controler = AppMainControler::getInstance();

    //LOG_DEBUG("setup : controler adr %x", &this_controler);

    this_controler.setup();
    //LOG_DEBUG("setup : wifi adr %x", this_controler.get_wifi_controler());
    //LOG_DEBUG("setup : unphone adr %x", this_controler.get_unphone_controler());
    //LOG_DEBUG("setup : mqtt adr %x", this_controler.get_mqtt_controler());
     //LOG_DEBUG("setup : sim adr %x", this_controler.get_simulation_controler());

    launch_tasks();
    //LOG_DEBUG("setup : done");
}


void loop()
{
    // LOG_DEBUG("main : loop");
    //lv_timer_handler();

    delay(10000);
    // sleep on power off
}
