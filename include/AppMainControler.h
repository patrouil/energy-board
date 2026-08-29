//
// Created by Patrick Rouillon on 25/01/2026.
//

#ifndef UPHONE1_MAINCONTROLER_H
#define UPHONE1_MAINCONTROLER_H
#include "AppTask.h"
#include "AppWifiControler.h"
#include "WelcomePage.h"

#define APP_CONTROLER_STACK_SIZE (4*1024)

class AppMainControler : public AppTask
{
private:
    static AppMainControler* controler;

    // WIFI section
    AppEventQueue* wifiOutQueue = nullptr;
    AppWifiControler* wifiManager = nullptr;
    WelcomePage* welcome_page = nullptr;
    //
    void manage_wifi_event();
    void manage_incoming_event();

public:
    AppMainControler();
    ~AppMainControler() override;

    void setup();

    WelcomePage* get_welcome_page();
    AppWifiControler* get_wifi_controler();

public:
    void run() override;
};


#endif //UPHONE1_MAINCONTROLER_H
