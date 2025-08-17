#pragma once

#include <raylib.h>
#include <raymath.h>

#include "guppy_r_lite.h"

Color guppyColor(float x) {
    const int idx = std::clamp(static_cast<int>(x * 255.0f + 0.5f), 0, 255);
    const auto& rgb = guppy_r_lite_colormap[idx];
    return Color{rgb[0], rgb[1], rgb[2], 255};
}

static constexpr Color COLOR_BACKGROUND = {16, 16, 16, 255};
static constexpr Color COLOR_RIBBON_BACKGROUND = {32, 32, 32, 255};
static constexpr Color COLOR_OBSTACLE = {96, 96, 96, 255};
static constexpr Color COLOR_PATH = {255, 255, 255, 255};

static constexpr Color COLOR_TEXT_CONTROLS = {240, 240, 240, 255};
static constexpr Color COLOR_TEXT_CONTROL_SELECT_BKGD = {240, 240, 240, 255};

const Color COLOR_GOAL_REACHED = guppyColor(0.0f);
const Color COLOR_GOAL_NOT_REACHED = guppyColor(0.8f);

const Color COLOR_NODE_COUNT = {240, 240, 240, 255};

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