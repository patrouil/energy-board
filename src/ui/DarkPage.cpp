//
// Created by Patrick Rouillon on 06/09/2026.
//
#include <unPhone.h>

#include "DarkPage.h"
#include "Log.h"


void DarkPage::create()
{
    Screen::create();
}

void DarkPage::show()
{
    Screen::show();
    unPhone::me->backlight(true);
    LOG_DEBUG("DarkPage::show : backlight on");
}

void DarkPage::hide()
{
    Screen::hide();
    unPhone::me->backlight(false);
    LOG_DEBUG("DarkPage::hide : backlight off");
}
