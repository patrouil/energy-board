//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "WelcomePage.h"

#include "Log.h"

#include "AppTask.h"
#include "Theme.h"

WelcomePage::WelcomePage(Display& disp)
    : Screen(disp)
{
    //LOG_DEBUG("WelcomePage::WelcomePage");
}

WelcomePage::~WelcomePage()
{
    LOG_DEBUG("WelcomePage::~WelcomePage");
    if (titleLabel) lv_obj_del(titleLabel);
    if (ipLabel) lv_obj_del(ipLabel);
    if (mqttLabel) lv_obj_del(mqttLabel);
    if (messageLabel) lv_obj_del(messageLabel);
    titleLabel = nullptr;
    ipLabel = nullptr;
    mqttLabel = nullptr;
    messageLabel = nullptr;
}

void WelcomePage::create()
{
    LOG_DEBUG("WelcomePage::create");
    Screen::create();
    lv_obj_set_style_bg_color(page, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN); // Bleu (palette)

    titleLabel = lv_label_create(this->page);
    APP_ASSERT(titleLabel != nullptr)
    lv_label_set_text(titleLabel, "About Energy Monitor");
    lv_obj_set_style_text_color(titleLabel, Theme::BUTTON_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_font(titleLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, Theme::LABEL_SPACING);

    // Ligne pour l'adresse IP
    ipLabel = lv_label_create(this->page);
    this->setIPAddress("--");
    lv_obj_set_style_text_font(ipLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(ipLabel, Theme::PRIMARY_COLOR, LV_PART_MAIN);

    lv_obj_align_to(ipLabel, titleLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, Theme::LABEL_SPACING);
    LOG_DEBUG("WelcomePage::create ip");

    // Ligne pour le statut MQTT
    mqttLabel = lv_label_create(this->page);
    this->setMQTTStatus("--");
    lv_obj_set_style_text_font(mqttLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(mqttLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_align_to(mqttLabel, ipLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, Theme::LABEL_SPACING);

    LOG_DEBUG("WelcomePage::create mqtt");

    // Ligne pour les messages
    messageLabel = lv_label_create(this->page);
    this->setMessage("Welcome");
    lv_obj_set_style_text_font(messageLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_align_to(messageLabel, mqttLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, Theme::LABEL_SPACING);
    lv_obj_set_style_text_color(ipLabel, Theme::SECONDARY_COLOR, LV_PART_MAIN);

    LOG_DEBUG("WelcomePage::create done");
}

void WelcomePage::setIPAddress(const char* ip)
{
    APP_ASSERT(ipLabel != nullptr);
    lv_label_set_text_fmt(ipLabel, "Adresse IP: %s", ip);
}

void WelcomePage::setMQTTStatus(const char* status)
{
    APP_ASSERT(mqttLabel != nullptr);
    lv_label_set_text_fmt(mqttLabel, "Statut MQTT: %s", status);
}

void WelcomePage::setMessage(const char* message)
{
    APP_ASSERT(messageLabel != nullptr);
    lv_label_set_text(messageLabel, message);
}
