//
// Created by Vibe Code on 2026.
//

#pragma once

#ifndef UPHONE1_SIMULATIONCONTROLER_H
#define UPHONE1_SIMULATIONCONTROLER_H

#include "AppTask.h"
#include "EventData.h"

class SimulationControler : public AppTask
{
    static const int32_t CONVERTION_FACTOR = 2;
    static const int32_t MAX_PRODUCTION = 4000;
    static const int32_t MAX_CONSUMPTION = 12000;
    static const int32_t STEP_VARIATION = 15;

    uint16_t currentPattern = 0;

    EventData simulationData;

    const AppEvent simulationEvent = AppEvent(AppEventType::SIMULATED_PRODUCTION, simulationData);

    int32_t variation(int32_t value, int32_t from, int32_t to);
    int32_t available();
    int32_t rand(int32_t maxVar);
    void computeRandomValues();

public:
    explicit SimulationControler(AppEventQueue* outQueue);
    ~SimulationControler() override;

    int switchPattern();

    void setup();
    void run() override;
};

#endif //UPHONE1_SIMULATIONCONTROLER_H
