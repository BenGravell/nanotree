#pragma once

#include <raylib.h>

#include <array>

// Physical elements
static constexpr float OBSTACLE_RADIUS = 60.0f;
static constexpr float DEVIATION_DISTANCE_MAX = 0.8f * OBSTACLE_RADIUS;
static constexpr float START_RADIUS = 15.0f;
static constexpr float GOAL_RADIUS = 30.0f;
static constexpr float CHEAP_PARENT_SEARCH_RADIUS = 1.5f * GOAL_RADIUS;
static constexpr float RADIUS_OF_CURVATURE_MIN = 0.95f * std::min(OBSTACLE_RADIUS, 2.0f * GOAL_RADIUS);
static constexpr float GOAL_SAMPLE_PROBABILITY = 0.02;

static constexpr float OBSTACLE_SPACING_MIN = 0.25f * OBSTACLE_RADIUS;
static constexpr float OBSTACLE_DEL_RADIUS = 15.0f;
static constexpr float START_CHANGED_DIST_MIN = 1.0f;

static constexpr Vector2 DEFAULT_START = {120, 450};
static constexpr Vector2 DEFAULT_GOAL = {1230, 690};

const std::vector<Vector2> DEFAULT_OBSTACLES = {
    {300, 360},
    {300, 390},
    {300, 420},
    {300, 450},
    {300, 480},
    {300, 510},
    {300, 540},
    {300, 570},
    {300, 600},
    {300, 630},
    {300, 660},
    {300, 690},
    {300, 720},
    {300, 750},
    {300, 780},
    {330, 360},
    {360, 360},
    {390, 360},
    {420, 360},
    {450, 360},
    {480, 30},
    {480, 60},
    {480, 90},
    {480, 120},
    {480, 150},
    {480, 180},
    {480, 210},
    {480, 240},
    {480, 270},
    {480, 300},
    {480, 330},
    {480, 360},
    {480, 540},
    {480, 570},
    {480, 600},
    {480, 630},
    {480, 660},
    {480, 690},
    {480, 720},
    {480, 750},
    {480, 780},
    {480, 810},
    {480, 840},
    {480, 870},
    {480, 900},
    {960, 120},
    {960, 150},
    {960, 180},
    {960, 210},
    {960, 240},
    {960, 270},
    {960, 300},
    {960, 330},
    {960, 360},
    {960, 390},
    {960, 420},
    {960, 450},
    {960, 510},
    {960, 540},
    {960, 570},
    {960, 600},
    {960, 630},
    {960, 660},
    {960, 690},
    {960, 720},
    {960, 750},
    {960, 780},
    {990, 120},
    {990, 450},
    {990, 780},
    {1020, 120},
    {1020, 450},
    {1020, 780},
    {1050, 120},
    {1050, 450},
    {1050, 780},
    {1080, 120},
    {1080, 450},
    {1080, 780},
    {1110, 120},
    {1110, 450},
    {1110, 780},
    {1140, 120},
    {1140, 300},
    {1140, 450},
    {1140, 600},
    {1140, 780},
    {1170, 120},
    {1170, 300},
    {1170, 450},
    {1170, 600},
    {1170, 780},
    {1200, 120},
    {1200, 300},
    {1200, 450},
    {1200, 600},
    {1200, 780},
    {1230, 120},
    {1230, 300},
    {1230, 450},
    {1230, 600},
    {1230, 780},
    {1260, 120},
    {1260, 300},
    {1260, 450},
    {1260, 600},
    {1260, 780},
    {1290, 120},
    {1290, 300},
    {1290, 450},
    {1290, 600},
    {1290, 780},
    {1320, 120},
    {1320, 300},
    {1320, 450},
    {1320, 600},
    {1320, 780},
    {1350, 120},
    {1350, 300},
    {1350, 450},
    {1350, 600},
    {1350, 780},
    {1380, 120},
    {1380, 300},
    {1380, 450},
    {1380, 600},
    {1380, 780},
    {1410, 120},
    {1410, 300},
    {1410, 450},
    {1410, 600},
    {1410, 780},
    {1440, 120},
    {1440, 150},
    {1440, 180},
    {1440, 210},
    {1440, 240},
    {1440, 270},
    {1440, 300},
    {1440, 450},
    {1440, 600},
    {1440, 630},
    {1440, 660},
    {1440, 690},
    {1440, 720},
    {1440, 750},
    {1440, 780},
    {1470, 450},
    {1500, 450},
    {1530, 450},
    {1560, 450}
};

static constexpr std::array<int, 14> NUM_SAMPLES_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
static constexpr std::array<int, 16> NUM_CARRYOVER_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000};

// UI sizes
static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;

static constexpr int RIBBON_NUM_COLS = 4;
static constexpr int RIBBON_NUM_ROWS = 2;
static constexpr int RIBBON_WIDTH = SCREEN_WIDTH;
static constexpr int RIBBON_HEIGHT = 180;
static constexpr int RIBBON_COL_WIDTH = RIBBON_WIDTH / RIBBON_NUM_COLS;
static constexpr int RIBBON_ROW_HEIGHT = RIBBON_HEIGHT / RIBBON_NUM_ROWS;

static constexpr int STATBAR_NUM_ROWS = 18;
static constexpr int STATBAR_WIDTH = 360;
static constexpr int STATBAR_HEIGHT = SCREEN_HEIGHT - RIBBON_HEIGHT;
static constexpr int STATBAR_ROW_HEIGHT = STATBAR_HEIGHT / STATBAR_NUM_ROWS;

static constexpr int ENVIRONMENT_WIDTH = SCREEN_WIDTH - STATBAR_WIDTH;
static constexpr int ENVIRONMENT_HEIGHT = STATBAR_HEIGHT;

static constexpr int GRID_THICKNESS = 3;
static constexpr int GRID_SPACING = 60;

static constexpr int TEXT_HEIGHT_CONTROL = 36;
static constexpr int TEXT_HEIGHT_STAT = 36;

static constexpr int RIBBON_ROW_1_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_CONTROL) / 2;
static constexpr int RIBBON_ROW_2_Y = ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;

static constexpr int TEXT_MARGIN_WIDTH = 20;

static constexpr int BUTTON_MARGIN = 10;
static constexpr int BUTTON_HEIGHT = RIBBON_ROW_HEIGHT - (2 * BUTTON_MARGIN);
static constexpr int BUTTON_WIDTH = (RIBBON_COL_WIDTH / 2) - (2 * BUTTON_MARGIN);

static constexpr int DELTA_BUTTON_HEIGHT = BUTTON_HEIGHT;
static constexpr int DELTA_BUTTON_WIDTH = DELTA_BUTTON_HEIGHT;

static constexpr int LINE_WIDTH_TREE = 5;
static constexpr int LINE_WIDTH_PATH = 15;
static constexpr int NODE_WIDTH_PATH = 30;

// TIMING
static constexpr float TIMING_WINDOW_SEC = 2.0f;