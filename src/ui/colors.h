#pragma once

#include <raylib.h>
#include <raymath.h>

#include "ui/guppy_r_lite.h"

Color guppyColor(const float x) {
    const int idx = std::clamp(static_cast<int>(x * 255.0f + 0.5f), 0, 255);
    const auto& rgb = guppy_r_lite_colormap[idx];
    return Color{rgb[0], rgb[1], rgb[2], 255};
}

static constexpr Color COLOR_GRAY_016 = {16, 16, 16, 255};
static constexpr Color COLOR_GRAY_032 = {32, 32, 32, 255};
static constexpr Color COLOR_GRAY_048 = {48, 48, 48, 255};
static constexpr Color COLOR_GRAY_064 = {64, 64, 64, 255};
static constexpr Color COLOR_GRAY_096 = {96, 96, 96, 255};
static constexpr Color COLOR_GRAY_128 = {128, 128, 128, 255};
static constexpr Color COLOR_GRAY_160 = {160, 160, 160, 255};
static constexpr Color COLOR_GRAY_192 = {192, 192, 192, 255};
static constexpr Color COLOR_GRAY_224 = {224, 224, 224, 255};
static constexpr Color COLOR_GRAY_240 = {240, 240, 240, 255};

const Color COLOR_GOOD = guppyColor(0.0f);
const Color COLOR_OK = guppyColor(1.0f);
const Color COLOR_BAD = guppyColor(0.8f);

static constexpr Color COLOR_BACKGROUND = COLOR_GRAY_016;
static constexpr Color COLOR_STAT_BAR_BACKGROUND = COLOR_GRAY_032;

static constexpr Color COLOR_SCREEN_BORDER = COLOR_GRAY_240;
static constexpr Color COLOR_STAT_BAR_BORDER = COLOR_SCREEN_BORDER;

static constexpr Color COLOR_OBSTACLE = COLOR_GRAY_096;
static constexpr Color COLOR_PATH_GOAL_REACHED = COLOR_GRAY_240;
static constexpr Color COLOR_PATH_GOAL_NOT_REACHED = GOLD;

static constexpr Color COLOR_GRID = COLOR_GRAY_048;

static constexpr Color COLOR_START = GOLD;
const Color COLOR_GOAL_REACHED = COLOR_GOOD;
const Color COLOR_GOAL_NOT_REACHED = COLOR_BAD;

const Color COLOR_STAT = COLOR_GRAY_240;
const Color COLOR_TIMING_STAT = COLOR_GRAY_160;

const Color COLOR_FPS_HIGH = COLOR_GOOD;
const Color COLOR_FPS_MID = COLOR_OK;
const Color COLOR_FPS_LOW = COLOR_BAD;

Color computeFpsColor(const int fps) {
    if (fps < 15) return COLOR_FPS_LOW;
    if (fps < 30) return COLOR_FPS_MID;
    return COLOR_FPS_HIGH;
}

Color computeCostColor(const float x) {
    static constexpr float r = 0.6f;
    return (x < 0.5f) ? guppyColor(r * x) : COLOR_GRAY_064;
}