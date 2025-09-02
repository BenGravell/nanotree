#pragma once

#include <raylib.h>

#include <array>

// UI options
static constexpr std::array<int, 14> NUM_SAMPLES_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
static constexpr std::array<int, 15> NUM_CARRYOVER_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};

// UI sizes
static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;
static constexpr Rectangle SCREEN_REC = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

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
static constexpr Rectangle ENVIRONMENT_REC = {ENVIRONMENT_X_MIN, ENVIRONMENT_Y_MIN, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT};

static constexpr int GRID_THICKNESS = BORDER_THICKNESS;
static constexpr int GRID_SPACING = CELL_SIZE;

static constexpr int BUTTON_SPACING_X = 15;
static constexpr int BUTTON_SPACING_Y = 15;

static constexpr float LINE_WIDTH_TREE_MIN = 2.0f;
static constexpr float LINE_WIDTH_TREE_MAX = 6.0f;
static constexpr int LINE_WIDTH_TREE_SIZE_MIN = 1000;
static constexpr int LINE_WIDTH_TREE_SIZE_MAX = 20000;
static constexpr float TREE_SIZE_INV_MIN = 1.0f / LINE_WIDTH_TREE_SIZE_MAX;
static constexpr float TREE_SIZE_INV_MAX = 1.0f / LINE_WIDTH_TREE_SIZE_MIN;

static constexpr int LINE_WIDTH_PATH = 15;
static constexpr int NODE_WIDTH_PATH = 30;

static constexpr int TEXT_HEIGHT = 0.6 * CELL_SIZE;
static constexpr int BIG_TEXT_HEIGHT = 0.8 * CELL_SIZE;
static constexpr int SMALL_TEXT_HEIGHT = 0.5 * CELL_SIZE;

static constexpr int BUTTON_ICON_SCALE = 5;

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

static constexpr Vector2 DEFAULT_START = {ENVIRONMENT_X_MIN + CELL_SIZE, ENVIRONMENT_Y_MIN + ENVIRONMENT_HEIGHT / 2};
static constexpr Vector2 DEFAULT_GOAL = {1350, 270};

const std::vector<Vector2> DEFAULT_OBSTACLES = {
    {1560, 540},
    {1500, 540},
    {1440, 540},
    {1380, 540},
    {1320, 540},
    {1260, 540},
    {1200, 540},
    {1140, 540},
    {1080, 540},
    {1020, 540},
    {1020, 480},
    {1020, 420},
    {1020, 360},
    {1020, 300},
    {1020, 240},
    {1020, 180},
    {1080, 180},
    {1140, 180},
    {1200, 180},
    {1260, 180},
    {1320, 180},
    {1380, 180},
    {1440, 180},
    {1440, 240},
    {1440, 300},
    {1440, 360},
    {1380, 360},
    {1320, 360},
    {1260, 360},
    {1200, 360},
    {1020, 600},
    {1020, 660},
    {1020, 720},
    {1020, 780},
    {1020, 840},
    {1020, 900},
    {1080, 900},
    {1140, 900},
    {1200, 900},
    {1260, 900},
    {1320, 900},
    {1380, 900},
    {1440, 900},
    {1440, 840},
    {1440, 780},
    {1440, 720},
    {1380, 720},
    {1320, 720},
    {1260, 720},
    {1200, 720},
    {780, 1080},
    {780, 1020},
    {780, 960},
    {780, 900},
    {780, 840},
    {780, 780},
    {780, 720},
    {780, 660},
    {780, 420},
    {780, 360},
    {780, 300},
    {780, 240},
    {780, 180},
    {780, 120},
    {780, 60},
    {780, 0},
    {720, 420},
    {660, 420},
    {600, 420},
    {540, 420},
    {540, 480},
    {540, 540},
    {540, 600},
    {540, 660},
    {540, 720},
    {540, 780},
    {540, 840},
    {540, 900}};