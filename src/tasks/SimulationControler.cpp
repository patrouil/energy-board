//
// Created by Vibe Code on 2026.
//

#include "SimulationControler.h"

#include <WiFi.h>
#include <esp_system.h>

#include "Log.h"

//  solarPower, gridPower
static constexpr int32_t testPatterns[][2] = {
    {0, 160}, // sleepy night
    {3000, -2000}, // over prod
    {1500, 500}, // normal context.
    {0, 3000}
};

#define MAX_SOLAR_POWER 3000

#define MAX( a, b) (a > b ? a : b)


SimulationControler::SimulationControler(AppEventQueue* outQueue) :
    AppTask("SimulationControler", APP_TASK_STACK_MIN, APP_TASK_PRIORITY_BACKEND, nullptr, outQueue)
{
    LOG_DEBUG("SimulationControler::SimulationControler");
    this->simulationData.maxGridConsuption = MAX_CONSUMPTION;
}

SimulationControler::~SimulationControler()
{
    LOG_DEBUG("SimulationControler::~SimulationControler");
}

int32_t SimulationControler::variation(int32_t value, int32_t from, int32_t to)
{
    int32_t r = rand(STEP_VARIATION);

    int32_t v = (value >= -100 && value <= 100) ? (value + r) : value + ((value * r) / 100L);
    //LOG_DEBUG("random is %d value %d v %d", r, value, v);

    v = (v > to) ? to : v;
    v = (v < from) ? from : v;
    return v;
}

int32_t SimulationControler::lowRatePower()
{
    return (this->simulationData.solarPower * CONVERTION_FACTOR);
}

// range is - UINT32_MAX/2 and +UINT32_MAX/2
// result value is between -100 and +100
// but bounded to maxval so -maxvar to *maxvar.
int32_t SimulationControler::rand(int32_t upperBound)
{
    int32_t v = static_cast<int32_t>(esp_random() % (2 * upperBound)) - upperBound;
    return v;
}

void SimulationControler::computeRandomValues()
{
    EventData &sim = this->simulationData;
    sim.lock();
    int32_t newProd = variation(sim.solarPower, 0,  MAX_SOLAR_POWER);
    sim.gridPower += (sim.solarPower - newProd); // add to grid the solar variation
    sim.gridPower = variation(sim.gridPower,
                                                        - sim.solarPower,
                                                        sim.maxGridConsuption); // then varry
    sim.solarPower = newProd;

    sim.lowRateMaxPower = this->lowRatePower();
    sim.homeConsumption = sim.gridPower + sim.solarPower;
    sim.maxGridConsuption = 2* MAX ( sim.lowRateMaxPower, sim.homeConsumption);
    sim.release();

  //  LOG_DEBUG("SimulationControler : solar=%d lowrate=%d grid=%d maxGrid=%d ",
    //          sim.solarPower, sim.lowRateMaxPower,
      //        sim.gridPower, sim.maxGridConsuption);
}

void SimulationControler::setup()
{
    EventData &sim = this->simulationData;

    sim.lock();
    sim.solarPower = MAX_PRODUCTION / 2;
    sim.gridPower = 0;
    sim.maxGridConsuption = MAX_CONSUMPTION;
    sim.lowRateMaxPower = lowRatePower();
    sim.homeConsumption = sim.gridPower + sim.solarPower;

    sim.release();
}

int SimulationControler::switchPattern()
{
    EventData &sim = this->simulationData;

    this->currentPattern = (this->currentPattern + 1) % (sizeof(testPatterns) / sizeof(testPatterns[0]));
    sim.lock();
    sim.solarPower = testPatterns[this->currentPattern][0];
    sim.gridPower = testPatterns[this->currentPattern][1];
    sim.lowRateMaxPower = lowRatePower();
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
