//
// Created by Patrick Rouillon on 23/01/2026.
//


#include "app_event.h"


AppEvent::AppEvent(int16_t id, const void* data) : id(id), data(data)
{
}

AppEvent::AppEvent(int16_t id) : id(id), data(nullptr)
{
}


int16_t AppEvent::getId() const
{
    return id;
}

const void* AppEvent::getData() const
{
    return data;
}
