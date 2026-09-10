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
    EventData simulationData;

    const AppEvent simulationEvent = AppEvent(AppEventType::SIMULATED_PRODUCTION, simulationData);

    void computeRandomValues();

public:
    explicit SimulationControler(AppEventQueue* outQueue);
    ~SimulationControler() override;

    void run() override;
};

#endif //UPHONE1_SIMULATIONCONTROLER_H
