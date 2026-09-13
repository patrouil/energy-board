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
    void lock();
    void release();

    // WIFI Task Data
    String ipAddress;

    // MQTT Task Data
    String mqttTopic;
    String mqttPayload;

    // MQ Data
    int32_t currentProduction = 0;
    int32_t currentConsumption = 0;
    int32_t maxConsumption = 0;
    int32_t maxProduction = 0;
    int32_t powerAvailable = 0;

    // unPhone date
    static const uint16_t BUTTON_1 = 0x01;
    static const uint16_t BUTTON_2 = 0x02;
    static const uint16_t BUTTON_3 = 0x04;
    uint16_t currentButton = 0;

private :
    SemaphoreHandle_t logMutex = xSemaphoreCreateMutex();
};
