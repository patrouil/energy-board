//
// Created by Patrick Rouillon on 25/01/2026.
//

#ifndef UPHONE1_MAINCONTROLER_H
#define UPHONE1_MAINCONTROLER_H
#include "AppTask.h"
#include "AppWifiControler.h"
#include "UnphoneControler.h"

class AppMainControler : public AppTask
{
private:
    AppMainControler();
    ~AppMainControler() override;

    // WIFI section
    AppEventQueue* wifiOutQueue = nullptr;
    AppWifiControler* wifiManager = nullptr;
    //
    void manage_wifi_event();
    void manage_unphone_event();

    // unphone section
    AppEventQueue* unphoneOutQueue = nullptr;
    UnphoneControler* unphoneManager = nullptr;

public:
    static AppMainControler& getInstance()
    {
        static AppMainControler instance;
        return instance;
    }

    void setup();

    AppWifiControler* get_wifi_controler() const
    {
        return wifiManager;
    }

    UnphoneControler* get_unphone_controler() const
    {
        return unphoneManager;
    }

    void run() override;
};


#endif //UPHONE1_MAINCONTROLER_H
