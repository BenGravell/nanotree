#pragma once

#include <raylib.h>
#include <raymath.h>

#include "ui/guppy_r_hq.h"

Color guppyColor(const float x) {
    static constexpr int n = guppy_r_hq_colormap.size();
    const int idx = std::clamp(static_cast<int>(x * float(n - 1) + 0.5f), 0, n - 1);
    const auto& rgb = guppy_r_hq_colormap[idx];
    return Color{rgb[0], rgb[1], rgb[2], 255};
}

static constexpr Color COLOR_GRAY_008 = {8, 8, 8, 255};
static constexpr Color COLOR_GRAY_048 = {48, 48, 48, 255};
static constexpr Color COLOR_GRAY_064 = {64, 64, 64, 255};
static constexpr Color COLOR_GRAY_096 = {96, 96, 96, 255};
static constexpr Color COLOR_GRAY_128 = {128, 128, 128, 255};
static constexpr Color COLOR_GRAY_160 = {160, 160, 160, 255};
static constexpr Color COLOR_GRAY_240 = {240, 240, 240, 255};

static constexpr Color COLOR_DARK = COLOR_GRAY_008;
static constexpr Color COLOR_LITE = COLOR_GRAY_240;

const Color COLOR_EMERALD = {21, 193, 100, 255};
const Color COLOR_AMBER = {243, 182, 30, 255};

const Color COLOR_GREAT = guppyColor(0.1f);
const Color COLOR_GOOD = COLOR_EMERALD;
const Color COLOR_WARN = COLOR_AMBER;
const Color COLOR_BAD = guppyColor(0.85f);

static constexpr Color COLOR_BACKGROUND = COLOR_DARK;
static constexpr Color COLOR_STAT_BAR_BACKGROUND = COLOR_DARK;

static constexpr Color COLOR_SCREEN_BORDER = COLOR_LITE;
static constexpr Color COLOR_STAT_BAR_BORDER = COLOR_SCREEN_BORDER;

static constexpr Color COLOR_OBSTACLE = COLOR_GRAY_096;
static constexpr Color COLOR_PATH_GOAL_REACHED = COLOR_LITE;
const Color COLOR_PATH_GOAL_NOT_REACHED = COLOR_WARN;

static constexpr Color COLOR_GRID = COLOR_GRAY_048;

const Color COLOR_START = COLOR_WARN;
const Color COLOR_GOAL_REACHED = COLOR_GREAT;
const Color COLOR_GOAL_NOT_REACHED = COLOR_BAD;

const Color COLOR_OBJECT_BRUSH_BKGD = Fade(COLOR_GRAY_128, 0.4f);
const Color COLOR_OBJECT_BRUSH_FRGD = Fade(COLOR_GRAY_160, 0.6f);

const Color COLOR_STAT = COLOR_LITE;
const Color COLOR_MINOR_STAT = COLOR_GRAY_160;

Color computeFrameTimeColor(const float duration) {
    if (duration < 0.020f) return COLOR_GREAT;
    if (duration < 0.050f) return COLOR_GOOD;
    if (duration < 0.100f) return COLOR_WARN;
    return COLOR_BAD;
}
