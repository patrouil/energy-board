//
// Created by Patrick Rouillon on 23/01/2026.
//



#include "app_event.h"

static const EventData dummyData;

AppEvent::AppEvent(AppEventType id, EventData & data) : id(id), data(data)
{
}

AppEvent::AppEvent(AppEventType id) : id(id), data(dummyData)
{
}


AppEventType AppEvent::getId() const
{
    return id;
}

const EventData& AppEvent::getData() const
{
    return data;
}
