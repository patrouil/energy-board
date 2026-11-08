//
// Created by Patrick Rouillon on 06/09/2026.
//

#pragma once

#include "Screen.h"

class DarkPage : public Screen
{
public:
    explicit DarkPage() = default;

    ~DarkPage() = default;
    void create() override;
    void show() override;
    void hide() override;
};

