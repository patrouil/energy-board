//
// Created by Patrick Rouillon on 25/01/2026.
//

#include "AppMainControler.h"

#include "Log.h"
#include "PageRouter.h"
#include "utils/wattHelper.h"


AppMainControler::AppMainControler() : AppTask("mainControler", APP_TASK_STACK_DEFAULT, APP_TASK_PRIORITY_BUSINESS,
                                               new AppEventQueue(APP_EVENT_QUEUE_DEFAULT_SIZE), nullptr)
{
}

AppMainControler::~AppMainControler()
{
    //LOG_DEBUG("AppMainControler::delete AppMainControler");

    if (simulationManager)
    {
        this->simulationManager->stop();
        delete this->simulationManager;
    }
    if (simulationOutQueue) delete this->simulationOutQueue;
    this->simulationManager = nullptr;
    this->simulationOutQueue = nullptr;

    if (mqttManager)
    {
        this->mqttManager->stop();
        delete this->mqttManager;
    }
    if (mqttOutQueue) delete this->mqttOutQueue;
    this->mqttManager = nullptr;
    this->mqttOutQueue = nullptr;

    if (wifiManager)
    {
        this->wifiManager->stop();
        delete this->wifiManager;
    }
    if (wifiOutQueue) delete this->wifiOutQueue;
    this->wifiManager = nullptr;
    this->wifiOutQueue = nullptr;

    if (this->unphoneManager)
    {
        this->unphoneManager->stop();
        delete this->unphoneManager;
    }
    if (this->unphoneOutQueue) delete this->unphoneOutQueue;
    this->unphoneManager = nullptr;
    this->unphoneOutQueue = nullptr;
}

bool AppMainControler::manage_wifi_event()
{
    if (this->wifiOutQueue->isEmpty())
        return false;

    PageRouter& router = PageRouter::getInstance();
    AppEvent ev(AppEventType::NONE);

    if (!this->wifiOutQueue->pop(&ev))
    {
        LOG_ERROR("AppMainControler::manage_wifi_event : unable to get event");
        return false;
    }
    //  LOG_DEBUG("AppMainControler::manage_wifi_event :%d", ev.getId());

    switch (ev.getId())
    {
    case AppEventType::WIFI_IDLE:
       // LOG_DEBUG("AppMainControler::APP_EVENT_WIFI_IDLE :%d", ev.getId());

        break;
    case AppEventType::WIFI_CONNECTED:
        {
            //      LOG_DEBUG("AppMainControler::APP_EVENT_WIFI_CONNECTED :%d", ev.getId());
            // notify MQTT and UI
            static String s = WiFi.localIP().toString();
            //    LOG_DEBUG("AppMainControler::APP_EVENT_WIFI_CONNECTED :%s %s", "display IP", s.c_str());
            StatusPage* p = router.getStatusPage();
            p->setIPAddress(s.c_str());
            p->setMessage("Connected");
            //p->show();
        }
        break;
    case AppEventType::WIFI_DISCONNECTED:
        {
            // LOG_DEBUG("AppMainControler::APP_EVENT_WIFI_DISCONNECTED :%d", ev.getId());
            // notidy MQTT and UI
            StatusPage* p = router.getStatusPage();
            p->setIPAddress("none");
            p->setMessage("diconnected");
        }
        break;
    default:
        break;
    }
    return true;
}

bool AppMainControler::manage_unphone_event()
{
    if (this->unphoneOutQueue->isEmpty())
        return false;
    PageRouter& router = PageRouter::getInstance();
    AppEvent ev(AppEventType::NONE);

    if (!this->unphoneOutQueue->pop(&ev))
    {
        LOG_ERROR("AppMainControler::manage_incoming_event : unable to get event");
        return false;
    }
    //  LOG_DEBUG("AppMainControler::manage_incoming_event :%d", ev.getId());

    switch (ev.getId())
    {
    case AppEventType::UNPHONE_BUTTON_PRESSED:
        {
                    LOG_DEBUG("AppMainControler::APP_EVENT_UNPHONE_BUTTON :%d %x", ev.getId(), ev.getData().currentButton);
            String b = "button";
            uint16_t v = ev.getData().currentButton;
            if (v & EventData::BUTTON_1) b.concat(" 1");
            if (v & EventData::BUTTON_2) b.concat(" 2");
            if (v & EventData::BUTTON_3) b.concat(" 3");

            router.getStatusPage()->setMessage(b.c_str());
            if (v & EventData::BUTTON_1)
            {
                router.getStatusPage()->show();
            }
            else if (v & EventData::BUTTON_2)
            {
                int p = this->get_simulation_controler()->switchPattern();
                // LOG_DEBUG("AppMainControler::manage_incoming_event : pattern %d", p);
                router.getDashboardPage()->show();
            }
            break;
        }
    case AppEventType::UNPHONE_BUTTON_RELEASED:
        {
            router.getStatusPage()->setMessage("button release");
            break;
        }
    default:
        break;
    }
    return true;
}

bool AppMainControler::manage_mqtt_event()
{
    if (this->mqttOutQueue == nullptr || this->mqttOutQueue->isEmpty())
        return false;

    PageRouter& router = PageRouter::getInstance();

    AppEvent ev(AppEventType::NONE);

    if (!this->mqttOutQueue->pop(&ev))
    {
        LOG_ERROR("AppMainControler::manage_mqtt_event : unable to get event");
        return false;
    }
    // LOG_DEBUG("AppMainControler::manage_mqtt_event :%d", ev.getId());

    switch (ev.getId())
    {
    case AppEventType::MQTT_IDLE:
        LOG_DEBUG("AppMainControler::MQTT_IDLE :%d", ev.getId());
        router.getStatusPage()->setMQTTStatus("idle");
        break;
    case AppEventType::MQTT_CONNECTED:
        {
            LOG_DEBUG("AppMainControler::MQTT_CONNECTED :%d", ev.getId());
            router.getStatusPage()->setMQTTStatus("connected");
        }
        break;
    case AppEventType::MQTT_DISCONNECTED:
        {
            LOG_DEBUG("AppMainControler::MQTT_DISCONNECTED :%d", ev.getId());
            router.getStatusPage()->setMQTTStatus("disconnected");
        }
        break;
    case AppEventType::MQTT_MESSAGE_RECEIVED:
        {
            LOG_DEBUG("AppMainControler::MQTT_MESSAGE_RECEIVED :%d %s", ev.getId(),
                      ev.getData().mqttTopic.c_str());
            String msg = ev.getData().mqttTopic + " : " + ev.getData().mqttPayload;
            router.getStatusPage()->setMessage(msg.c_str());
        }
        break;
    default:
        break;
    }
    return true;
}

bool AppMainControler::manage_simulation_event()
{
    static bool first = true;
    if (this->simulationOutQueue == nullptr || this->simulationOutQueue->isEmpty())
        return false;

    AppEvent ev(AppEventType::NONE);

    if (!this->simulationOutQueue->pop(&ev))
    {
        LOG_ERROR("AppMainControler::manage_simulation_event : unable to get event");
        return false;
    }
    // LOG_DEBUG("AppMainControler::manage_simulation_event :%d", ev.getId());
    PageRouter& router = PageRouter::getInstance();

    switch (ev.getId())
    {
    case AppEventType::SIMULATED_PRODUCTION:
        {
            const EventData& d = ev.getData();
            d.lock();
            char message[256];
            //LOG_DEBUG("AppMainControler::SIMULATED_PRODUCTION solar=%d lowrate=%d  grid=%d home=%d ",
            //          d.solarPower, d.lowRateMaxPower, d.gridPower, d.homeConsumption);
            sniprintf(message, sizeof(message), "solar=%d lowrate=%d  grid=%d home=%d ",
                      d.solarPower, d.lowRateMaxPower, d.gridPower, d.homeConsumption);
            DashboardPage* p = router.getDashboardPage();
            p->setBottomMessage(message);

            p->setPowerRange(0, d.maxGridPower);
            p->setProductionValue(d.solarPower, d.lowRateMaxPower, d.gridPower);
            p->setHomeConsumption(d.homeConsumption);
            d.release();
            if ( first)
            {
                p->show();
                first = false;
            }
        }
        break;
    default:
        break;
    }
    return true;
}

void AppMainControler::setup()
{
    //    LOG_DEBUG("AppMainControler::setup :");

    this->wifiOutQueue = new AppEventQueue(APP_EVENT_QUEUE_SMALL_SIZE);
    APP_ASSERT(this->wifiOutQueue != nullptr);
    AppConfig& config = AppConfig::getInstance();
    this->wifiManager = new AppWifiControler(&(config.wifi), this->wifiOutQueue);
    APP_ASSERT(this->wifiManager != nullptr);
    this->wifiManager->setup();
    //  LOG_DEBUG("AppMainControler::setup wifi %x:", this->wifiManager);

    this->unphoneOutQueue = new AppEventQueue(APP_EVENT_QUEUE_SMALL_SIZE);
    APP_ASSERT(this->unphoneOutQueue != nullptr);

    this->unphoneManager = new UnphoneControler(this->unphoneOutQueue);
    APP_ASSERT(this->unphoneManager != nullptr);
    //LOG_DEBUG("AppMainControler::setup unphone %x:", this->unphoneManager);
    this->unphoneManager->setup();
#if 0  // not used yet
    this->mqttOutQueue = new AppEventQueue(APP_EVENT_QUEUE_DEFAULT_SIZE);
    APP_ASSERT(this->mqttOutQueue != nullptr);
    this->mqttManager = new MqttControler(&(config.mqtt), this->mqttOutQueue);
    APP_ASSERT(this->mqttManager != nullptr);
    LOG_DEBUG("AppMainControler::setup mqtt %x:", this->mqttManager);
    this->mqttManager->setup();

#endif

    this->simulationOutQueue = new AppEventQueue(APP_EVENT_QUEUE_DEFAULT_SIZE);
    APP_ASSERT(this->simulationOutQueue != nullptr);
    this->simulationManager = new SimulationControler(this->simulationOutQueue);
    APP_ASSERT(this->simulationManager != nullptr);
    //LOG_DEBUG("AppMainControler::setup simulation %x:", this->simulationManager);
    this->simulationManager->setup();
}

void AppMainControler::subscribeMqtt(const char* topic)
{
    if (this->mqttManager != nullptr)
    {
        this->mqttManager->subscribe(topic);
    }
}

void AppMainControler::run()
{
    while (true)
    {
        // checkStack();
        // if an event is received do not sleep go for the next one
        lv_timer_handler();
        if (manage_wifi_event() ||
            (manage_unphone_event()) ||
            (manage_mqtt_event()) ||
            manage_simulation_event())
        {
            yield();
        }
        else
        {
            this->sleep(100);
        }
    }
}
