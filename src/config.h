#pragma once

#include <raylib.h>

#include <array>

// Math
static constexpr float TWO_PI = 2.0f * M_PI;

// UI options
static constexpr std::array<int, 14> NUM_SAMPLES_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
static constexpr std::array<int, 16> NUM_CARRYOVER_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000};

// UI sizes
static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;

static constexpr int CELL_SIZE = 60;

static constexpr int BORDER_THICKNESS = 3;

static constexpr int STAT_BAR_WIDTH = 6 * CELL_SIZE;
static constexpr int CTRL_BAR_WIDTH = STAT_BAR_WIDTH;
static constexpr int ENVIRONMENT_WIDTH = SCREEN_WIDTH - STAT_BAR_WIDTH - CTRL_BAR_WIDTH;

static constexpr int STAT_BAR_HEIGHT = SCREEN_HEIGHT;
static constexpr int CTRL_BAR_HEIGHT = STAT_BAR_HEIGHT;
static constexpr int ENVIRONMENT_HEIGHT = SCREEN_HEIGHT;

static constexpr int STAT_BAR_X_MIN = 0;
static constexpr int STAT_BAR_X_MAX = STAT_BAR_X_MIN + STAT_BAR_WIDTH;
static constexpr int STAT_BAR_Y_MIN = 0;
static constexpr int STAT_BAR_Y_MAX = STAT_BAR_Y_MIN + STAT_BAR_HEIGHT;
static constexpr Rectangle STAT_BAR_REC = {STAT_BAR_X_MIN, STAT_BAR_Y_MIN, STAT_BAR_WIDTH, STAT_BAR_HEIGHT};
static constexpr int STAT_BAR_ROW_HEIGHT = CELL_SIZE;
static constexpr int STAT_BAR_NUM_ROWS = STAT_BAR_HEIGHT / STAT_BAR_ROW_HEIGHT;

static constexpr int CTRL_BAR_X_MIN = STAT_BAR_WIDTH + ENVIRONMENT_WIDTH;
static constexpr int CTRL_BAR_X_MAX = CTRL_BAR_X_MIN + CTRL_BAR_WIDTH;
static constexpr int CTRL_BAR_Y_MIN = 0;
static constexpr int CTRL_BAR_Y_MAX = CTRL_BAR_Y_MIN + CTRL_BAR_HEIGHT;
static constexpr Rectangle CTRL_BAR_REC = {CTRL_BAR_X_MIN, CTRL_BAR_Y_MIN, CTRL_BAR_WIDTH, CTRL_BAR_HEIGHT};
static constexpr int CTRL_BAR_ROW_HEIGHT = STAT_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_NUM_ROWS = CTRL_BAR_HEIGHT / CTRL_BAR_ROW_HEIGHT;

static constexpr int ENVIRONMENT_X_MIN = STAT_BAR_X_MAX;
static constexpr int ENVIRONMENT_X_MAX = CTRL_BAR_X_MIN;
static constexpr int ENVIRONMENT_Y_MIN = 0;
static constexpr int ENVIRONMENT_Y_MAX = ENVIRONMENT_HEIGHT;

static constexpr int GRID_THICKNESS = BORDER_THICKNESS;
static constexpr int GRID_SPACING = CELL_SIZE;

static constexpr int BUTTON_SPACING_X = 6;
static constexpr int BUTTON_SPACING_Y = 6;

static constexpr int LINE_WIDTH_TREE_MIN = 1;
static constexpr int LINE_WIDTH_TREE_MAX = 5;
static constexpr int LINE_WIDTH_TREE_SIZE_MIN = 2000;
static constexpr int LINE_WIDTH_TREE_SIZE_MAX = 10000;
static constexpr float TREE_SIZE_INV_MIN = 1.0f / LINE_WIDTH_TREE_SIZE_MAX;
static constexpr float TREE_SIZE_INV_MAX = 1.0f / LINE_WIDTH_TREE_SIZE_MIN;

static constexpr int LINE_WIDTH_PATH = 15;
static constexpr int NODE_WIDTH_PATH = 30;

static constexpr int TEXT_HEIGHT = 0.7 * CELL_SIZE;

// UI TIMES
static constexpr float MOMENT_DURATION = 0.100f;

// TIMING
static constexpr float TIMING_WINDOW_SEC = 1.0f;

// Physical elements
static constexpr float OBSTACLE_RADIUS = 60.0f;
static constexpr float DEVIATION_DISTANCE_MAX = 0.8f * OBSTACLE_RADIUS;
static constexpr float GOAL_RADIUS = 30.0f;
static constexpr float START_RADIUS = GOAL_RADIUS;
static constexpr float CHEAP_PARENT_SEARCH_RADIUS = 1.5f * GOAL_RADIUS;
static constexpr float RADIUS_OF_CURVATURE_MIN = 0.95f * std::min(OBSTACLE_RADIUS, 2.0f * GOAL_RADIUS);
static constexpr float GOAL_SAMPLE_PROBABILITY = 0.05;

static constexpr float OBSTACLE_SPACING_MIN = 0.25f * OBSTACLE_RADIUS;
static constexpr float OBSTACLE_DEL_RADIUS = 30.0f;
static constexpr float START_CHANGED_DIST_MIN = 1.0f;

static constexpr Vector2 DEFAULT_START = {ENVIRONMENT_X_MIN + 120, ENVIRONMENT_Y_MIN + 540};
static constexpr Vector2 DEFAULT_GOAL = {ENVIRONMENT_X_MAX - 120, ENVIRONMENT_Y_MIN + 540};

const std::vector<Vector2> DEFAULT_OBSTACLES = {};

// const std::vector<Vector2> DEFAULT_OBSTACLES = {
//     {300, 360},
//     {300, 390},
//     {300, 420},
//     {300, 450},
//     {300, 480},
//     {300, 510},
//     {300, 540},
//     {300, 570},
//     {300, 600},
//     {300, 630},
//     {300, 660},
//     {300, 690},
//     {300, 720},
//     {300, 750},
//     {300, 780},
//     {330, 360},
//     {360, 360},
//     {390, 360},
//     {420, 360},
//     {450, 360},
//     {480, 30},
//     {480, 60},
//     {480, 90},
//     {480, 120},
//     {480, 150},
//     {480, 180},
//     {480, 210},
//     {480, 240},
//     {480, 270},
//     {480, 300},
//     {480, 330},
//     {480, 360},
//     {480, 540},
//     {480, 570},
//     {480, 600},
//     {480, 630},
//     {480, 660},
//     {480, 690},
//     {480, 720},
//     {480, 750},
//     {480, 780},
//     {480, 810},
//     {480, 840},
//     {480, 870},
//     {480, 900},
//     {960, 120},
//     {960, 150},
//     {960, 180},
//     {960, 210},
//     {960, 240},
//     {960, 270},
//     {960, 300},
//     {960, 330},
//     {960, 360},
//     {960, 390},
//     {960, 420},
//     {960, 450},
//     {960, 480},
//     {960, 510},
//     {960, 540},
//     {960, 570},
//     {960, 600},
//     {960, 630},
//     {960, 660},
//     {960, 690},
//     {960, 720},
//     {960, 750},
//     {960, 780},
//     {990, 120},
//     {990, 450},
//     {990, 780},
//     {1020, 120},
//     {1020, 450},
//     {1020, 780},
//     {1050, 120},
//     {1050, 450},
//     {1050, 780},
//     {1080, 120},
//     {1080, 450},
//     {1080, 780},
//     {1110, 120},
//     {1110, 450},
//     {1110, 780},
//     {1140, 120},
//     {1140, 300},
//     {1140, 450},
//     {1140, 600},
//     {1140, 780},
//     {1170, 120},
//     {1170, 300},
//     {1170, 450},
//     {1170, 600},
//     {1170, 780},
//     {1200, 120},
//     {1200, 300},
//     {1200, 450},
//     {1200, 600},
//     {1200, 780},
//     {1230, 120},
//     {1230, 300},
//     {1230, 450},
//     {1230, 600},
//     {1230, 780},
//     {1260, 120},
//     {1260, 300},
//     {1260, 450},
//     {1260, 600},
//     {1260, 780},
//     {1290, 120},
//     {1290, 300},
//     {1290, 450},
//     {1290, 600},
//     {1290, 780},
//     {1320, 120},
//     {1320, 300},
//     {1320, 450},
//     {1320, 600},
//     {1320, 780},
//     {1350, 120},
//     {1350, 300},
//     {1350, 450},
//     {1350, 600},
//     {1350, 780},
//     {1380, 120},
//     {1380, 300},
//     {1380, 450},
//     {1380, 600},
//     {1380, 780},
//     {1410, 120},
//     {1410, 300},
//     {1410, 450},
//     {1410, 600},
//     {1410, 780},
//     {1440, 120},
//     {1440, 150},
//     {1440, 180},
//     {1440, 210},
//     {1440, 240},
//     {1440, 270},
//     {1440, 300},
//     {1440, 450},
//     {1440, 600},
//     {1440, 630},
//     {1440, 660},
//     {1440, 690},
//     {1440, 720},
//     {1440, 750},
//     {1440, 780},
//     {1470, 450},
//     {1500, 450},
//     {1530, 450},
//     {1560, 450}};