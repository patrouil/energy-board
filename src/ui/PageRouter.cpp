//
// Created by Patrick Rouillon on 06/09/2026.
//

#include "PageRouter.h"
#include "Log.h"

#include "WelcomePage.h"
#include "BootPage.h"

PageRouter::PageRouter()

{
    for (uint8_t i = 0; i < static_cast<uint8_t>(ScreenId::SCREEN_COUNT); ++i) {
        screens[i] = nullptr;
    }
}

PageRouter::~PageRouter()
{
    for (uint8_t i = 0; i < static_cast<uint8_t>(ScreenId::SCREEN_COUNT); ++i) {
        if (screens[i] != nullptr) {
            delete screens[i];
            screens[i] = nullptr;
        }
    }

}

Screen* PageRouter::get_screen(ScreenId id)
{
    uint8_t index = static_cast<uint8_t>(id);
    if (index >= static_cast<uint8_t>(ScreenId::SCREEN_COUNT)) {
        LOG_ERROR("PageRouter::get_screen: Invalid screen id %d", index);
        return nullptr;
    }

    if (screens[index] == nullptr) {
        switch (id) {
        case ScreenId::WELCOME_PAGE:
            screens[index] = new WelcomePage();
            screens[index]->create();
            LOG_DEBUG("PageRouter::get_screen: Created WELCOME_PAGE");
            break;
        case ScreenId::BOOT_PAGE:
            screens[index] = new BootPage();
            screens[index]->create();
            LOG_DEBUG("PageRouter::get_screen: Created BOOT_PAGE");
            break;
        default:
            LOG_ERROR("PageRouter::get_screen: Unknown screen id %d", index);
            return nullptr;
        }
    }
    return screens[index];
}

void PageRouter::display_screen(ScreenId id)
{
    Screen* screen = get_screen(id);
    if (screen != nullptr) {
        screen->show();
        LOG_DEBUG("PageRouter::display_screen: Displaying screen %d", static_cast<uint8_t>(id));
    } else {
        LOG_ERROR("PageRouter::display_screen: Failed to display screen %d", static_cast<uint8_t>(id));
    }
}
