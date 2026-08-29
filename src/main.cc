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


//#include <Adafruit_SPIFlash.h> // for LDF
#include <Log.h>
#define LV_USE_LOG 1
#include <lvgl.h>                       // LVGL //////////////////////////////
#define CONFIG_IDF_TARGET_ESP32S3 1

#include <TFT_eSPI.h>
#include <AppWifiControler.h>
#include <Display.h>
#include "AppMainControler.h"
#include "Theme.h"

// create an unPhone; add a custom version of Arduino's map command for
// translating from touchscreen coordinates to LCD coordinates


// GLOBAL VARIABLES.

unPhone this_phone_u = unPhone();
Display this_display = Display();
AppConfig* this_config = nullptr;
AppMainControler* this_controler = nullptr;
uint8_t currentButton = 0xFF;

// END OF GLOBALS


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

    // start of changes for unPhone ////////////////////////////////////////////
    bool touched = this_phone_u.tsp->touched();

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        TS_Point p(-1, -1, -1);
        p = this_phone_u.tsp->getPoint();

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

        long xscld = my_mapper((long)p.x, xMin, xMax, 0, (long)displayWidth);
        long yscld = // Y is inverted on rotation 1 (landscape, buttons right)
            ((long)displayHeight) -
            my_mapper((long)p.y, yMin, yMax, 0, (long)displayHeight);
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
    BaseType_t result;
    TaskHandle_t handle;
    result = xTaskCreate(
        [](void* pvParameters)
        {
            // Lambda sans capture
            this_controler->run();
        },
        this_controler->get_task_name(),
        this_controler->get_stack_size(),
        nullptr,
        this_controler->get_priority(),
        &handle
    );

    APP_ASSERT(result == pdPASS)
    AppWifiControler* w = this_controler->get_wifi_controler();
    result = xTaskCreate(
        [](void* pvParameters)
        {
            // Lambda sans capture
            this_controler->get_wifi_controler()->run();
        },
        w->get_task_name(),
        w->get_stack_size(),
        nullptr,
        w->get_priority(),
        &handle
    );
    APP_ASSERT(result == pdPASS)
}

void setup()
{
    unPhone& ph = this_phone_u;
    Display& dsp = this_display;

    //   Serial.begin(115200); /* prepare for possible serial debug */
    //   while (!Serial);
    //    Serial.setDebugOutput(true);

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    LOG_INIT(Log::DEBUG, 115200);

    ph.begin();
    ph.tftp = (void*)&(dsp.get_tft_screen());

    ph.tsp->setRotation(1);
    ph.backlight(true);
    // after unPhone init
    LOG_DEBUG("display  init");

    try
    {
        dsp.init();
        Theme::getInstance().init(dsp.lvgl_display());
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Exception standard capturée : %s", e.what());
    }
    catch (...)
    {
        LOG_ERROR("Une exception inconnue s'est produite.");
    }
    lv_timer_create([](lv_timer_t* timer)
    {
        lv_timer_handler(); // ✅ Appelé automatiquement par LVGL
    }, 50, nullptr); // ✅ Toutes les 50ms

#if 0
    /* Simple boot screen */
    String LVGL_Arduino = "Loading Energy Monitor! ";
    String v = "Hardware version " + String(this_phone_u.version());
    LVGL_Arduino.concat(v);
    lv_obj_t* boot_page = lv_obj_create(dsp.getActiveScreen());
    lv_coord_t width = lv_disp_get_hor_res(dsp.lvgl_display());
    lv_coord_t height = lv_disp_get_ver_res(dsp.lvgl_display());
    lv_obj_set_size(boot_page, width, height);
    lv_obj_t* label = lv_label_create(boot_page);
    lv_label_set_text(label, LVGL_Arduino.c_str());
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_scr_load(boot_page);
    // should do some refresh here.
#endif
    LOG_DEBUG("let start config");

    this_config = new AppConfig(&this_phone_u);
    this_config->loadConfig();
    if (!this_config->wifi.ready)
    {
        LOG_DEBUG("setup : default wifi");
        this_config->defaultWifi();
    }

    LOG_DEBUG("setup : create tasks");

    this_controler = new AppMainControler();
    LOG_DEBUG("setup : controler adr %x", this_controler);

    this_controler->setup();
    LOG_DEBUG("setup : wifi adr %x", this_controler->get_wifi_controler());
    //this_controler->get_wifi_controler()->start();
    //this_controler->start();
    launch_tasks();
    LOG_DEBUG("setup : done");
}

const AppEvent button1Event = AppEvent(AppEventType::UNPHONE_BUTTON1  );
const AppEvent button2Event = AppEvent( AppEventType::UNPHONE_BUTTON2 );
const AppEvent button3Event = AppEvent( AppEventType::UNPHONE_BUTTON3 );

void handleButtonPress()
{
    unPhone& ph = this_phone_u;

    if (ph.button1() && currentButton != unPhone::BUTTON1)
    {
        this_controler->getIncomingEventQueue()->push(button1Event);
        currentButton = unPhone::BUTTON1;
    }
    else if (ph.button2() && currentButton != unPhone::BUTTON2)
    {
        this_controler->getIncomingEventQueue()->push(button2Event);
        currentButton = unPhone::BUTTON2;
    }
    else if (ph.button3() && currentButton != unPhone::BUTTON3)
    {
        this_controler->getIncomingEventQueue()->push(button3Event);
        currentButton = unPhone::BUTTON3;
    }
    else
    {
        currentButton = 0xFF;
    }
    LOG_DEBUG("handleButtonPress : button is %d", currentButton);
}

void loop()
{
    LOG_DEBUG("main : loop");
    handleButtonPress();
    delay(3000);
}
