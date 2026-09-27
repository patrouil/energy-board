//
// Created by Vibe Code on 16/09/2026.
//
#include "Display.h"
#include "PowerScale.h"
#include "AppTask.h"
#include "Log.h"
#include "Theme.h"

static const lv_coord_t POWER_SCALE_HEIGHT = 60;
static const lv_coord_t POWER_SCALE_MAJOR_LEN = 18;
static const lv_coord_t POWER_SCALE_MINOR_LEN = 10;
static const lv_coord_t POWER_SCALE_LABEL_Y = 22;

PowerScale::~PowerScale()
{
    LOG_DEBUG("PowerScale destruction");
    if (container) lv_obj_del(container);
    container = nullptr;
    axisLine = nullptr;
    tickCount = 0;
}

lv_obj_t* PowerScale::create(lv_obj_t* parent)
{
    APP_ASSERT(parent != nullptr);

    container = lv_obj_create(parent);
    APP_ASSERT(container != nullptr);
    lv_obj_remove_style_all(container);
    lv_obj_set_style_pad_all(container, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    lv_coord_t parentWidth = lv_obj_get_content_width(parent);
    if (parentWidth <= 0)
    {
        parentWidth = lv_obj_get_width(parent);
    }
    if (parentWidth <= 0)
    {
        parentWidth = Display::displayWidth;
    }
    lv_obj_set_size(container, parentWidth, POWER_SCALE_HEIGHT);

    axisPoints[0].x = 0;
    axisPoints[0].y = 0;
    axisPoints[1].x = parentWidth;
    axisPoints[1].y = 0;

    axisLine = lv_line_create(container);
    APP_ASSERT(axisLine != nullptr);
    lv_line_set_points(axisLine, axisPoints, 2);
    lv_obj_set_style_line_width(axisLine, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(axisLine, Theme::TEXT_COLOR, LV_PART_MAIN);
    lv_obj_set_style_line_rounded(axisLine, false, LV_PART_MAIN);

    // lines and labels are statically allocated. Just hiden or shown
    for (int32_t tickIndex = 0; tickIndex < MAX_TICKS; ++tickIndex)
    {
        lv_coord_t x = tickIndex;

        uint16_t idx = tickIndex;
        tickPoints[idx * 2].x = x;
        tickPoints[idx * 2].y = 0;
        tickPoints[idx * 2 + 1].x = x;
        tickPoints[idx * 2 + 1].y = POWER_SCALE_MINOR_LEN;

        lv_obj_t* line = lv_line_create(container);
        APP_ASSERT(line != nullptr);
        lv_line_set_points(line, &tickPoints[idx * 2], 2);
        lv_obj_set_style_line_width(line, 1, LV_PART_MAIN);
        lv_obj_set_style_line_color(line, Theme::TEXT_COLOR, LV_PART_MAIN);
        lv_obj_set_style_line_rounded(line, false, LV_PART_MAIN);
        lv_obj_add_flag(line, LV_OBJ_FLAG_HIDDEN); // hidden by default
        tickLines[idx] = line;
        tickValues[idx] = tickIndex;


        lv_obj_t* label = lv_label_create(container);
        APP_ASSERT(label != nullptr);
        char* lblptr = tickLabelsValues[idx];
        snprintf(lblptr, sizeof(lblptr), "%d", idx);
        lv_label_set_text_static(label, lblptr);
        lv_obj_set_style_text_color(label, Theme::TEXT_COLOR, LV_PART_MAIN);
        // lv_obj_set_style_text_font(label, Theme::SMALL_FONT, LV_PART_MAIN);
        // lv_obj_set_style_bg_opa(label, LV_OPA_90, LV_PART_MAIN); // no background
        lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN); // hidden by default

        lv_obj_align(label, LV_ALIGN_TOP_LEFT, x, POWER_SCALE_LABEL_Y);
        tickLabels[idx] = label;
    }
    tickCount = 0; // all hidden
    return container;
}

void PowerScale::setRange(int32_t min, int32_t max)
{
    if (min >= max)
    {
        LOG_WARN("PowerScale::setRange invalid range min=%d max=%d", min, max);
        return;
    }
    this->minPower = min;
    this->maxPower = max;
    LOG_DEBUG("PowerScale::setRange min=%d max=%d", min, max);
    rebuild();
}

void PowerScale::setTickInterval(int32_t intervalWatts)
{
    if (intervalWatts <= 0)
    {
        LOG_WARN("PowerScale::setTickInterval invalid interval=%d", intervalWatts);
        return;
    }
    this->tickInterval = intervalWatts;
    LOG_DEBUG("PowerScale::setTickInterval interval=%d", intervalWatts);
}

void PowerScale::setMajorTickEvery(uint16_t tickCount)
{
    if (tickCount == 0)
    {
        LOG_WARN("PowerScale::setMajorTickEvery invalid value=%d", tickCount);
        return;
    }
    this->majorTickEvery = tickCount;
}

void PowerScale::setLabelShow(bool show)
{
    this->labelShow = show;
}

lv_coord_t PowerScale::valueToX(int32_t value, lv_coord_t width) const
{
    int32_t span = maxPower - minPower;
    if (span <= 0) return 0;
    int32_t x = (value - minPower) * width / span;
    if (x < 0) x = 0;
    if (x > width) x = width;
    return static_cast<lv_coord_t>(x);
}

void PowerScale::destroyTicks()
{
    LOG_DEBUG("PowerScale::destroyTicks cout=%d ", tickCount);
    uint16_t i;
    xSemaphoreTake(logMutex, portMAX_DELAY);

    try
    {
        for (i = 0; i < tickCount; ++i)
        {
            if (tickLines[i] != nullptr && lv_obj_is_valid(tickLines[i]))
            {
                lv_obj_add_flag(tickLines[i], LV_OBJ_FLAG_HIDDEN);
            }
            if (tickLabels[i] != nullptr && lv_obj_is_valid(tickLabels[i]))
            {
                lv_obj_add_flag(tickLabels[i], LV_OBJ_FLAG_HIDDEN);
                tickLabelsValues[i][0] = '\0';
            }
        }
    }
    catch (const std::exception& e)
    {
        LOG_ERROR("EPowerScale::destroyTicks exception standard %d : %s", i, e.what());
    }

    catch (...)
    {
        LOG_ERROR("PowerScale::destroyTicks exception %d ", i);
    }
    tickCount = 0;
    xSemaphoreGive(logMutex);
}

void PowerScale::rebuild()
{
    if (!container) return;
    destroyTicks();

    if (tickInterval <= 0 || maxPower <= minPower) return;
    LOG_DEBUG("PowerScale::rebuild ");

    lv_coord_t width = lv_obj_get_width(container);
    if (width <= 0)
    {
        width = lv_obj_get_content_width(container);
    }
    if (width <= 0) return;

    if (axisLine != nullptr)
    {
        axisPoints[1].x = width;
        lv_line_set_points(axisLine, axisPoints, 2);
    }

    LOG_DEBUG("PowerScale::rebuild width %d", width);
    xSemaphoreTake(logMutex, portMAX_DELAY);

    tickCount = 0;
    int32_t tickIndex = 0;
    for (int32_t v = minPower; v <= maxPower && tickCount < MAX_TICKS; v += tickInterval, ++tickIndex)
    {
        lv_coord_t x = valueToX(v, width);
        bool isMajor = (tickIndex % majorTickEvery) == 0;

        uint16_t idx = tickCount;
        tickPoints[idx * 2].x = x;
        tickPoints[idx * 2].y = 0;
        tickPoints[idx * 2 + 1].x = x;
        tickPoints[idx * 2 + 1].y = isMajor ? POWER_SCALE_MAJOR_LEN : POWER_SCALE_MINOR_LEN;

        lv_obj_t* line = tickLines[idx];
        APP_ASSERT(line != nullptr);
        lv_line_set_points(line, &tickPoints[idx * 2], 2);
        lv_obj_set_style_line_width(line, isMajor ? 2 : 1, LV_PART_MAIN);
        lv_obj_set_style_line_color(line, Theme::TEXT_COLOR, LV_PART_MAIN);
        lv_obj_set_style_line_rounded(line, false, LV_PART_MAIN);
        lv_obj_clear_flag(line, LV_OBJ_FLAG_HIDDEN);
        tickValues[idx] = v;

        if (isMajor && labelShow)
        {
            lv_obj_t* label = tickLabels[idx];
            APP_ASSERT(label != nullptr);
            char* text = tickLabelsValues[idx];
            snprintf(text, MAX_LABEL, "%d", v / 1000L);
            // lv_label_set_text_static(label, text); already done at init.
            lv_obj_align(label, LV_ALIGN_TOP_LEFT, x, POWER_SCALE_LABEL_Y);
            lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
        }
        tickCount++;
    }

    lv_obj_update_layout(container);
    // update layout to have labels width.
    for (uint16_t i = 0; i < tickCount; ++i)
    {
        lv_obj_t* label = tickLabels[i];
        if (label == nullptr) continue;
        lv_coord_t x = valueToX(tickValues[i], width);
        lv_coord_t labelW = lv_obj_get_width(label);
        //LOG_DEBUG("PowerScale::rebuild : label width  %d", labelW);
        if (x - labelW / 2 > 0)
            lv_obj_set_pos(label, x - labelW / 2, POWER_SCALE_LABEL_Y);
    }
    xSemaphoreGive(logMutex);

    lv_obj_invalidate(container);
    LOG_DEBUG("PowerScale::rebuild ticks=%d width=%d", tickCount, width);
}
