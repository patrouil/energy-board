//
// Created by Patrick Rouillon on 04/02/2026.
//

#include "StatusPage.h"

#include <string.h>

#include "Log.h"

#include "AppTask.h"
#include "Theme.h"

StatusPage::~StatusPage()
{
    //LOG_DEBUG("StatusPage::~StatusPage");
    if (titleLabel) lv_obj_del(titleLabel);
    if (wifiSSIDLabel) lv_obj_del(wifiSSIDLabel);
    if (ipLabel) lv_obj_del(ipLabel);
    if (mqttServerLabel) lv_obj_del(mqttServerLabel);
    if (mqttLabel) lv_obj_del(mqttLabel);
    if (messageLabel) lv_obj_del(messageLabel);
    titleLabel = nullptr;
    wifiSSIDLabel = nullptr;
    ipLabel = nullptr;
    mqttServerLabel = nullptr;
    mqttLabel = nullptr;
    messageLabel = nullptr;
}

void StatusPage::create()
{
    LOG_DEBUG("StatusPage::create");
    Screen::create();
    // lv_obj_set_style_bg_color(page, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN); // Bleu (palette)

    titleLabel = lv_label_create(this->page);
    APP_ASSERT(titleLabel != nullptr)
    lv_label_set_text(titleLabel, "About Energy Monitor");
    lv_obj_set_style_text_color(titleLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    //lv_obj_set_style_text_font(titleLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, Theme::LABEL_SPACING);

    // Ligne pour le SSID du WiFi
    wifiSSIDLabel = lv_label_create(this->page);
    this->setWifiSSID("--");
    //lv_obj_set_style_text_font(wifiSSIDLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(wifiSSIDLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_align_to(wifiSSIDLabel, titleLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, Theme::LABEL_SPACING);

    // Ligne pour l'adresse IP
    ipLabel = lv_label_create(this->page);
    this->setIPAddress("--");
    //lv_obj_set_style_text_font(ipLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(ipLabel, Theme::TEXT_COLOR, LV_PART_MAIN);

    lv_obj_align_to(ipLabel, wifiSSIDLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, Theme::LABEL_SPACING);
    //LOG_DEBUG("StatusPage::create ip");

    // Ligne pour le serveur MQTT
    mqttServerLabel = lv_label_create(this->page);
    this->setMqttServer("--");
    //lv_obj_set_style_text_font(mqttServerLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(mqttServerLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_align_to(mqttServerLabel, ipLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, Theme::LABEL_SPACING);

    // Ligne pour le statut MQTT
    mqttLabel = lv_label_create(this->page);
    this->setMQTTStatus("--");
    //lv_obj_set_style_text_font(mqttLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(mqttLabel, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_align_to(mqttLabel, mqttServerLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, Theme::LABEL_SPACING);

    //LOG_DEBUG("StatusPage::create mqtt");

    // Ligne pour les messages
    messageLabel = lv_label_create(this->page);
    this->setMessage("Welcome");
    // lv_obj_set_style_text_font(messageLabel, Theme::DEFAULT_FONT, LV_PART_MAIN);
    lv_obj_set_style_text_color(messageLabel, Theme::TEXT_COLOR, LV_PART_MAIN);

    lv_obj_align_to(messageLabel, mqttLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, Theme::LABEL_SPACING);

    //LOG_DEBUG("StatusPage::create done");
}

void StatusPage::setWifiSSID(const char* ssid)
{
    if ( wifiSSIDLabel ) lv_label_set_text_fmt(wifiSSIDLabel, "SSID: %s", ssid);
}

void StatusPage::setIPAddress(const char* ip)
{
    if ( ipLabel) lv_label_set_text_fmt(ipLabel, "Adresse IP: %s", ip);
}

void StatusPage::setMqttServer(const char* server)
{
    if ( mqttServerLabel ) lv_label_set_text_fmt(mqttServerLabel, "Serveur MQTT: %s", server);
}

void StatusPage::setMQTTStatus(const char* status)
{
    if ( mqttLabel ) lv_label_set_text_fmt(mqttLabel, "Statut MQTT: %s", status);
}

void StatusPage::setMessage(const char* message)
{
    messageCount = 0;
    if ( messageLabel ) lv_label_set_text(messageLabel, message);
}

void StatusPage::addMessage(const char* message)
{
    if ( messageLabel == nullptr || message == nullptr ) return;

    if ( messageCount >= MAX_MESSAGES )
    {
        for (uint8_t i = 1; i < MAX_MESSAGES; ++i)
        {
            strncpy(messageLines[i - 1], messageLines[i], MESSAGE_MAX_LEN + 1);
        }
        messageCount = MAX_MESSAGES - 1;
    }
    strncpy(messageLines[messageCount], message, MESSAGE_MAX_LEN);
    messageLines[messageCount][MESSAGE_MAX_LEN] = '\0';
    messageCount++;

    this->updateMessageBuffer();
    lv_label_set_text(messageLabel, messageBuffer);
}

void StatusPage::updateMessageBuffer()
{
    uint16_t offset = 0;
    for (uint8_t i = 0; i < messageCount; ++i)
    {
        if ( i > 0 )
        {
            messageBuffer[offset] = '\n';
            offset++;
        }
        uint16_t len = strlen(messageLines[i]);
        memcpy(messageBuffer + offset, messageLines[i], len);
        offset += len;
    }
    messageBuffer[offset] = '\0';
}
