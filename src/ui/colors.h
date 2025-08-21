#pragma once

#include <raylib.h>
#include <raymath.h>

#include "ui/guppy_r_lite.h"

Color guppyColor(const float x) {
    const int idx = std::clamp(static_cast<int>(x * 255.0f + 0.5f), 0, 255);
    const auto& rgb = guppy_r_lite_colormap[idx];
    return Color{rgb[0], rgb[1], rgb[2], 255};
}

static constexpr Color COLOR_GRAY_0 = {16, 16, 16, 255};
static constexpr Color COLOR_GRAY_1 = {32, 32, 32, 255};
static constexpr Color COLOR_GRAY_2 = {64, 64, 64, 255};
static constexpr Color COLOR_GRAY_3 = {96, 96, 96, 255};
static constexpr Color COLOR_GRAY_4 = {160, 160, 160, 255};
static constexpr Color COLOR_GRAY_5 = {192, 192, 192, 255};
static constexpr Color COLOR_GRAY_6 = {240, 240, 240, 255};
static constexpr Color COLOR_GRAY_7 = {255, 255, 255, 255};

static constexpr Color COLOR_BACKGROUND = COLOR_GRAY_0;
static constexpr Color COLOR_RIBBON_BACKGROUND = COLOR_GRAY_1;
static constexpr Color COLOR_OBSTACLE = COLOR_GRAY_3;
static constexpr Color COLOR_PATH = COLOR_GRAY_7;

static constexpr Color COLOR_TEXT_CONTROLS = COLOR_GRAY_6;
static constexpr Color COLOR_TEXT_CONTROL_SELECT_BKGD = COLOR_GRAY_6;

static constexpr Color COLOR_BUTTON_BACKGROUND_INACTIVE = COLOR_GRAY_2;
static constexpr Color COLOR_BUTTON_BACKGROUND_ACTIVE = COLOR_GRAY_6;
static constexpr Color COLOR_BUTTON_BORDER_INACTIVE = COLOR_BUTTON_BACKGROUND_ACTIVE;
static constexpr Color COLOR_BUTTON_BORDER_ACTIVE = COLOR_BUTTON_BACKGROUND_INACTIVE;
static constexpr Color COLOR_BUTTON_CONTENT_INACTIVE = COLOR_BUTTON_BORDER_INACTIVE;
static constexpr Color COLOR_BUTTON_CONTENT_ACTIVE = COLOR_GRAY_1;

static constexpr Color COLOR_GRID_MINOR = COLOR_GRAY_1;
static constexpr Color COLOR_GRID_MAJOR = COLOR_GRAY_2;

static constexpr Color COLOR_START = COLOR_GRAY_6;
const Color COLOR_GOAL_REACHED = guppyColor(0.0f);
const Color COLOR_GOAL_NOT_REACHED = guppyColor(0.8f);

const Color COLOR_NODE_COUNT = COLOR_GRAY_6;

const Color COLOR_FPS_HIGH = guppyColor(0.0f);
const Color COLOR_FPS_MID = GOLD;
const Color COLOR_FPS_LOW = guppyColor(0.8f);

Color computeFpsColor(const int fps) {
    if (fps < 15) return COLOR_FPS_LOW;
    if (fps < 30) return COLOR_FPS_MID;
    return COLOR_FPS_HIGH;
}

Color computeCostColor(const float x) {
    return guppyColor(x);
}