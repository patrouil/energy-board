//
// Created by Patrick Rouillon on 08/09/2026.
//

#pragma once

#include <freertos/FreeRTOS.h>
#include <semphr.h>
#include <Log.h>  // for String class in Arduino.h


/* common payload to shre across events.
 * There is one instance per event generator.
 * This is a kind of share memory
 * All data are public
 */
class EventData
{
public :

    EventData()
    {
        memset(ipAddress, 0, sizeof(ipAddress));
    }

    void lock() const;
    void release() const;

    // WIFI Task Data
    char ipAddress[24];

    void setIp(const char* ip)
    {
        strncpy(ipAddress, ip, sizeof(ipAddress));
    }

    void setIp(const String& ip)
    {
        strncpy(ipAddress, ip.c_str(), sizeof(ipAddress));
    }

    // MQTT Task Data
    String mqttTopic;
    String mqttPayload;

    // MQ Data
    int32_t solarPower = 0;
    int32_t gridPower = 0;
    int32_t maxGridConsuption = 0;
    int32_t lowRateMaxPower = 0;
    int32_t homeConsumption = 0;


    // unPhone date
    static const uint16_t BUTTON_1 = 0x01;
    static const uint16_t BUTTON_2 = 0x02;
    static const uint16_t BUTTON_3 = 0x04;
    uint16_t currentButton = 0;

private :
    SemaphoreHandle_t logMutex = xSemaphoreCreateMutex();
};
