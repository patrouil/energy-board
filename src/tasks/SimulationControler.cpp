//
// Created by Vibe Code on 2026.
//

#include "SimulationControler.h"

#include <WiFi.h>
#include <esp_system.h>

#include "Log.h"

SimulationControler::SimulationControler(AppEventQueue* outQueue) :
    AppTask("SimulationControler", APP_TASK_STACK_DEFAULT, APP_TASK_PRIORITY_BACKEND, nullptr, outQueue)
{
    LOG_DEBUG("SimulationControler::SimulationControler");
}

SimulationControler::~SimulationControler()
{
    LOG_DEBUG("SimulationControler::~SimulationControler");
}

void SimulationControler::computeRandomValues()
{
    static const int32_t MAX_PRODUCTION = 5000;
    static const int32_t MAX_CONSUMPTION = 5000;

    int32_t production = (int32_t)(esp_random() % (MAX_PRODUCTION + 1));
    int32_t maxConsumption = (int32_t)(esp_random() % (MAX_CONSUMPTION + 1));
    int32_t consumption = (int32_t)(esp_random() % (maxConsumption + 1));
    int32_t powerAvailable = production - consumption;

    this->simulationData.lock();
    this->simulationData.currentProduction = production;
    this->simulationData.currentConsuption = consumption;
    this->simulationData.maxConsuption = maxConsumption;
    this->simulationData.powerAvailable = powerAvailable;
    this->simulationData.release();

    LOG_DEBUG("SimulationControler : production=%d consumption=%d max=%d available=%d",
              production, consumption, maxConsumption, powerAvailable);
}

void SimulationControler::run()
{
    while (true)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            computeRandomValues();
            this->sendEvent(this->simulationEvent);
        }
        else
        {
            LOG_DEBUG("SimulationControler : wifi not connected, skipping");
        }

        this->sleep(3000);
    }
}
