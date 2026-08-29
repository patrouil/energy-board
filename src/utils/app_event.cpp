//
// Created by Patrick Rouillon on 23/01/2026.
//


#include "app_event.h"


AppEvent::AppEvent(AppEventType id, const void* data) : id(id), data(data)
{
}

AppEvent::AppEvent(AppEventType id) : id(id), data(nullptr)
{
}


AppEventType AppEvent::getId() const
{
    return id;
}

const void* AppEvent::getData() const
{
    return data;
}
