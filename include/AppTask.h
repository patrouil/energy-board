//
// Created by Patrick Rouillon on 24/01/2026.
//

#ifndef UPHONE1_APPTASK_H
#define UPHONE1_APPTASK_H


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Log.h>
#include "app_event_queue.h"
/*
 *SAMPLE
*
*class MyTask : public AppTask {
public:
    MyTask() : AppTask("MyTask", 2048, APP_TASKS_PRIORITY_DEFAULT, APP_EVENT_QUEUE_DEFAULT_SIZE) {}

protected:
    void run() override {
        AppEvent event(0, nullptr);
        while (true) {
            if (receiveEvent(event)) {
                // Traiter l'événement
                int16_t id = event.getId();
                void* data = event.getData();
                // Logique spécifique à la tâche
            }
            this->sleep(10);
        }
}
};

// Utilisation dans le code principal
MyTask myTask;
void setup() {
    myTask.start();
    int exampleData = 42;
    AppEvent newEvent(1, &exampleData);
    myTask.sendEvent(newEvent);
}

void loop() {}

*/

#define APP_TASK_PRIORITY_BACKEND   (tskIDLE_PRIORITY+1)
#define APP_TASK_PRIORITY_BUSINESS  (tskIDLE_PRIORITY+2)
#define APP_TASK_PRIORITY_UI        (configMAX_PRIORITIES-2)
#define APP_TASKS_PRIORITY_DEFAULT  (tskIDLE_PRIORITY+2)

#define APP_TASK_STACK_DEFAULT (2048)


class AppTask
{
public:
    AppTask(const char* taskName, uint16_t stackSize, UBaseType_t priority = APP_TASKS_PRIORITY_DEFAULT,
            AppEventQueue* incomingQueue = nullptr, AppEventQueue* outgoingQueue = nullptr);
    virtual ~AppTask();
    void start();

    void suspend() const;
    void resume() const;
    void stop() const;
    void yield();
    void sleep(int milli);
    UBaseType_t checkStack();

    AppEventQueue* getIncomingEventQueue() const
    {
        return this->incomingQueue;
    }

    AppEventQueue* getOutgoingEventQueue() const
    {
        return this->outgoingQueue;
    }
    virtual void run() = 0;
protected:


    bool sendEvent(const AppEvent& event) const;
    bool receiveEvent(AppEvent* event);

    TaskHandle_t taskHandle = nullptr;
    AppEventQueue* incomingQueue;
    AppEventQueue* outgoingQueue;

private:
    static void taskWrapper(void* parameters);

    const char* taskName;

public:
    const char* get_task_name() const
    {
        return taskName;
    }

    uint16_t get_stack_size() const
    {
        return stackSize;
    }

    UBaseType_t get_priority() const
    {
        return priority;
    }

private:
    uint16_t stackSize;
    UBaseType_t priority;
};


#endif //UPHONE1_APPTASK_H
