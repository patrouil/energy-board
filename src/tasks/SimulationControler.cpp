//
// Created by Vibe Code on 2026.
//

#include "SimulationControler.h"

#include <WiFi.h>
#include <esp_system.h>

#include "Log.h"

//  currentProduction , currentConsumption
static constexpr int32_t testPatterns[3][2] = {
    {0, 160}, // sleepy night
    {3000, -2000}, // over prod
    {1500, 500}, // normal context.
};

SimulationControler::SimulationControler(AppEventQueue* outQueue) :
    AppTask("SimulationControler", APP_TASK_STACK_MIN, APP_TASK_PRIORITY_BACKEND, nullptr, outQueue)
{
    LOG_DEBUG("SimulationControler::SimulationControler");
    this->simulationData.maxConsumption = MAX_CONSUMPTION;
    this->simulationData.maxProduction =  MAX_PRODUCTION;  // warning prod is positive but have a negative impact on balance.

}

SimulationControler::~SimulationControler()
{
    LOG_DEBUG("SimulationControler::~SimulationControler");
}

int32_t SimulationControler::variation(int32_t value, int32_t from, int32_t to)
{
    int32_t r = rand(STEP_VARIATION);

    int32_t v = (value >= -100 && value <= 100) ? (value + r) : value + ((value * r) / 100L);
    LOG_DEBUG("random is %d value %d v %d", r, value, v);

    v = (v > to) ? to : v;
    v = (v < from) ? from : v;
    return v;
}

int32_t SimulationControler::available()
{
    return (this->simulationData.currentProduction * CONVERTION_FACTOR) - this->simulationData.currentConsumption;
}

// range is - UINT32_MAX/2 and +UINT32_MAX/2
// result value is between -100 and +100
// but bounded to maxval so -maxvar to *maxvar.
int32_t SimulationControler::rand(int32_t maxVar)
{
    int32_t v = static_cast<int32_t>(esp_random() % (2 * maxVar)) - maxVar;

    return v;
}

void SimulationControler::computeRandomValues()
{
    EventData &sim = this->simulationData;
    sim.lock();
    int32_t newProd = variation(sim.currentProduction, 0,  sim.maxProduction);
    sim.currentConsumption += (sim.currentProduction - newProd); // add prod variation
    sim.currentConsumption = variation(sim.currentConsumption,
                                                        - sim.currentProduction,
                                                        sim.maxConsumption); // then varry
    sim.currentProduction = newProd;

    sim.powerAvailable = this->available();
    sim.release();

    LOG_DEBUG("SimulationControler : production=%d available=%d consumption=%d max=%d ",
              sim.currentProduction, sim.powerAvailable,
              sim.currentConsumption, sim.maxConsumption);
}

void SimulationControler::setup()
{
    EventData &sim = this->simulationData;

    sim.lock();
    sim.currentProduction = MAX_PRODUCTION / 2;
    sim.currentConsumption = 0;
    sim.maxConsumption = MAX_CONSUMPTION;
    sim.powerAvailable = available();
    sim.release();
}

int SimulationControler::switchPattern()
{
    EventData &sim = this->simulationData;

    this->currentPattern = (this->currentPattern + 1) % (sizeof(testPatterns) / sizeof(testPatterns[0]));
    sim.lock();
    sim.currentProduction = testPatterns[this->currentPattern][0];
    sim.currentConsumption = testPatterns[this->currentPattern][1];
    sim.powerAvailable = available();
    sim.release();

    return this->currentPattern;
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

        this->sleep(5000);
    }
}
