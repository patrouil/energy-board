//
// Created by Patrick Rouillon on 24/01/2026.
//

#include "AppTask.h"

#include "Log.h"

AppTask::AppTask(const char* taskName, uint16_t stackSize, UBaseType_t priority, AppEventQueue* inQueue,
                 AppEventQueue* outQueue)
    : taskName(taskName), stackSize(stackSize), priority(priority), incomingQueue(inQueue), outgoingQueue(outQueue)
{
}

AppTask::~AppTask()
{
}

void AppTask::start()
{
    LOG_DEBUG("AppTask::start : %s ", taskName);

    xTaskCreate(
        taskWrapper,
        taskName,
        stackSize,
        this,
        priority,
        &taskHandle
    );
}

void AppTask::suspend() const
{
    vTaskSuspend(taskHandle);
}

void AppTask::resume() const
{
    vTaskResume(taskHandle);
}

void AppTask::stop() const
{
    vTaskDelete(taskHandle);
}

void AppTask::yield()
{
    LOG_DEBUG("AppTask::yield %s", this->taskName);

    taskYIELD();
}

void AppTask::sleep(int milli)
{
    //LOG_DEBUG("AppTask::sleep %s", this->taskName);

    vTaskDelay(pdMS_TO_TICKS(milli));
}

bool AppTask::sendEvent(const AppEvent& event) const
{
    LOG_DEBUG("AppTask::sendEvent %x to %x", event.getId(), this->outgoingQueue);

    return this->outgoingQueue->push(event);
}

bool AppTask::receiveEvent(AppEvent* event)
{
    return this->incomingQueue->pop(event);
}

void AppTask::taskWrapper(void* parameters)
{
    LOG_DEBUG("AppTask::taskWrapper : %x", parameters);
    try
    {
        AppTask* task = static_cast<AppTask*>(parameters);
        LOG_DEBUG("AppTask::taskWrapper :run %s", task->taskName);
        //task->run();
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("Exception standard capturée : %s", e.what());
    }
    catch (...)
    {
        LOG_ERROR("Une exception inconnue s'est produite.");
    }
}

UBaseType_t AppTask::checkStack()
{
    UBaseType_t ret = uxTaskGetStackHighWaterMark(this->taskHandle);

    LOG_DEBUG("AppTask::checkStack : %s %d", this->taskName, ret);

    return ret;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   char* pcTaskName)
{
    UBaseType_t ret = uxTaskGetStackHighWaterMark(xTask);
    LOG_FATAL("AppTask::overflow : %s %x", pcTaskName, ret);
    APP_ASSERT(false);
}
