#pragma once

#include <raylib.h>

#include <array>

// Physical elements
static constexpr float OBSTACLE_RADIUS = 80.0f;
static constexpr float DEVIATION_DISTANCE_MAX = 0.8f * OBSTACLE_RADIUS;
static constexpr float START_RADIUS = 20.0f;
static constexpr float GOAL_RADIUS = 40.0f;
static constexpr float CHEAP_PARENT_SEARCH_RADIUS = 1.5f * GOAL_RADIUS;
static constexpr float RADIUS_OF_CURVATURE_MIN = 0.95f * std::min(OBSTACLE_RADIUS, 2.0f * GOAL_RADIUS);
static constexpr float GOAL_SAMPLE_PROBABILITY = 0.02;

static constexpr float OBSTACLE_SPACING_MIN = 10.0f;
static constexpr float OBSTACLE_DEL_RADIUS = 20.0f;

static constexpr int GRID_THICKNESS = 3;
static constexpr int GRID_SPACING = 100;

static constexpr Vector2 DEFAULT_START = {100, 500};
static constexpr Vector2 DEFAULT_GOAL = {600, 500};

const std::vector<Vector2> DEFAULT_OBSTACLES = {
    {400, 1000},
    {400, 960},
    {400, 920},
    {400, 880},
    {400, 840},
    {400, 800},
    {400, 760},
    {400, 720},
    {400, 680},
    {400, 640},
    {400, 600},
    {400, 560},
    {400, 520},
    {400, 480},
    {400, 440},
    {400, 400},
    {400, 360},
    {400, 320},
    {400, 280},
    {400, 240},
    {400, 200},
    {440, 200},
    {480, 200},
    {520, 200},
    {560, 200},
    {600, 200},
    {640, 200},
    {680, 200},
    {720, 200},
    {760, 200},
    {800, 200},
    {800, 240},
    {800, 280},
    {800, 320},
    {800, 360},
    {800, 400},
    {800, 440},
    {800, 480},
    {800, 520},
    {800, 560},
    {800, 600},
    {800, 640},
    {800, 680},
    {800, 720},
    {800, 760},
    {800, 800},
    {1200, 0},
    {1200, 40},
    {1200, 80},
    {1200, 120},
    {1200, 160},
    {1200, 200},
    {1200, 240},
    {1200, 280},
    {1200, 320},
    {1200, 360},
    {1200, 400},
    {1200, 440},
    {1200, 480},
    {1200, 520},
    {1200, 560},
    {1200, 600},
    {1200, 640},
    {1200, 680},
    {1200, 720},
    {1200, 760},
    {1200, 800},
    {1240, 800},
    {1280, 800},
    {1320, 800},
    {1360, 800},
    {1400, 800},
    {1440, 800},
    {1480, 800},
    {1520, 800},
    {1560, 800},
    {1600, 800},
    {1600, 760},
    {1600, 720},
    {1600, 680},
    {1600, 640},
    {1600, 600},
    {1600, 560},
    {1600, 520},
    {1600, 480},
    {1600, 440},
    {1600, 400},
    {1600, 360},
    {1600, 320},
    {1600, 280},
    {1600, 240},
    {1600, 200}};

static constexpr std::array<int, 14> NUM_SAMPLES_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
static constexpr std::array<int, 16> NUM_CARRYOVER_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000};

// UI sizes
static constexpr int ENVIRONMENT_WIDTH = 2000;
static constexpr int ENVIRONMENT_HEIGHT = 1000;
static constexpr int RIBBON_NUM_COLS = 4;
static constexpr int RIBBON_NUM_ROWS = 2;
static constexpr int RIBBON_WIDTH = ENVIRONMENT_WIDTH;
static constexpr int RIBBON_HEIGHT = 200;
static constexpr int RIBBON_COL_WIDTH = RIBBON_WIDTH / RIBBON_NUM_COLS;
static constexpr int RIBBON_ROW_HEIGHT = RIBBON_HEIGHT / RIBBON_NUM_ROWS;

static constexpr int SCREEN_WIDTH = ENVIRONMENT_WIDTH;
static constexpr int SCREEN_HEIGHT = ENVIRONMENT_HEIGHT + RIBBON_HEIGHT;

static constexpr int TEXT_HEIGHT_CONTROL_MODE = 0.4 * RIBBON_ROW_HEIGHT;
static constexpr int TEXT_HEIGHT_CONTROL_KEYMAP = 0.2 * RIBBON_ROW_HEIGHT;
static constexpr int TEXT_HEIGHT_STAT = 0.5 * RIBBON_ROW_HEIGHT;

static constexpr int LINE_WIDTH_TREE = 5;
static constexpr int LINE_WIDTH_PATH = 15;
static constexpr int NODE_WIDTH_PATH = 30;
