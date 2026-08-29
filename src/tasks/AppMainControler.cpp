//
// Created by Patrick Rouillon on 25/01/2026.
//

#include "AppMainControler.h"

#include "Log.h"

AppMainControler* AppMainControler::controler = nullptr;

AppMainControler::AppMainControler() : AppTask("mainControler", APP_CONTROLER_STACK_SIZE, APP_TASK_PRIORITY_BUSINESS,
                                               nullptr, nullptr)
{
    AppMainControler::controler = this;
    LOG_DEBUG("AppMainControler::AppMainControler");
}

AppMainControler::~AppMainControler()
{
    LOG_DEBUG("AppMainControler::delete AppMainControler");

    this->wifiManager->disconnect();
}

void AppMainControler::manage_wifi_event()
{
    if (this->wifiOutQueue->isEmpty())
        return;

    AppEvent ev(AppEventType::NONE);

    if (!this->wifiOutQueue->pop(&ev))
    {
        LOG_ERROR("AppMainControler::manage_wifi_event : unable to get event");
        return;
    }
    LOG_DEBUG("AppMainControler::manage_wifi_event :%d", ev.getId());

    switch (ev.getId())
    {
    case  AppEventType::WIFI_IDLE  :
        LOG_DEBUG("AppMainControler::APP_EVENT_WIFI_IDLE :%d", ev.getId());

        this->get_welcome_page()->show();

        break;
    case AppEventType::WIFI_CONNECTED :
        {
            LOG_DEBUG("AppMainControler::APP_EVENT_WIFI_CONNECTED :%d", ev.getId());
            // notify MQTT and UI
            WelcomePage* p = this->get_welcome_page();
            LOG_DEBUG("AppMainControler::APP_EVENT_WIFI_CONNECTED :%s", "display IP'");

            //const char *ip = WiFi.localIP().toString().c_str();
            p->setIPAddress("IP is");
            p->setMessage("Connected");
            this->get_welcome_page()->show();

        }
        break;
    case AppEventType::WIFI_DISCONNECTED :
        {
            LOG_DEBUG("AppMainControler::APP_EVENT_WIFI_DISCONNECTED :%d", ev.getId());
            // notidy MQTT and UI
            WelcomePage* p = get_welcome_page();
            p->setIPAddress("none");
            p->setMessage("diconnected");
        }
        break;
    default:
        break;
    }
}


void AppMainControler::manage_incoming_event()
{
    if (this->incomingQueue->isEmpty())
        return;

    AppEvent ev(AppEventType::NONE );

    if (!this->incomingQueue->pop(&ev))
    {
        LOG_ERROR("AppMainControler::manage_incoming_event : unable to get event");
        return;
    }
    LOG_DEBUG("AppMainControler::manage_incoming_event :%d", ev.getId());

    switch (ev.getId())
    {
    case AppEventType::UNPHONE_BUTTON1 :
        LOG_DEBUG("AppMainControler::APP_EVENT_UNPHONE_BUTTON1 :%d", ev.getId());

        // this->get_welcome_page()->show();

        break;
    case AppEventType::UNPHONE_BUTTON2 :
        LOG_DEBUG("AppMainControler::APP_EVENT_UNPHONE_BUTTON2 :%d", ev.getId());

        // this->get_welcome_page()->show();

        break;
    case AppEventType::UNPHONE_BUTTON3 :
        LOG_DEBUG("AppMainControler::APP_EVENT_UNPHONE_BUTTON2 :%d", ev.getId());

        this->get_welcome_page()->show();
        break;
    default:
        break;
    }

}

void AppMainControler::setup()
{
    LOG_DEBUG("AppMainControler::setup :");

    this->wifiOutQueue = new AppEventQueue(APP_EVENT_QUEUE_DEFAULT_SIZE);
    LOG_DEBUG("AppMainControler::setup queue %x:", this->wifiOutQueue);

    APP_ASSERT(this->wifiOutQueue != nullptr);
    this->wifiManager = new AppWifiControler(&AppConfig::appConfig->wifi, this->wifiOutQueue);
    APP_ASSERT(this->wifiManager != nullptr);
    this->wifiManager->setup();
    LOG_DEBUG("AppMainControler::setup wifi %x:", this->wifiManager);
}

WelcomePage* AppMainControler::get_welcome_page()
{
    LOG_DEBUG("AppMainControler::get_welcome_Page %x:", welcome_page);
    if (this->welcome_page == nullptr)
    {
        this->welcome_page = new WelcomePage(*Display::me);
        LOG_DEBUG("AppMainControler::get_welcome_Page %x :", welcome_page);

        this->welcome_page->create();
    }
    APP_ASSERT(this->welcome_page != nullptr);

    return welcome_page;
}


AppWifiControler* AppMainControler::get_wifi_controler()
{
    LOG_DEBUG("AppMainControler::get_wifi_controler :");
    if (this->wifiManager == nullptr)
    {
        APP_ASSERT(this->wifiManager != nullptr);
    }
    return wifiManager;
}

void AppMainControler::run()
{
    LOG_DEBUG("AppMainControler::run :");

    //AppWifiControler* w = this->get_wifi_controler();
    //w->start();
    while (true)
    {
        LOG_DEBUG("AppMainControler::loop :");
        //taskDISABLE_INTERRUPTS();
        // checkStack();
        //manage_wifi_event();
        //manage_incoming_event();
        //Display::me->refresh();
        //taskENABLE_INTERRUPTS();

        this->sleep(5000);
    }
}
