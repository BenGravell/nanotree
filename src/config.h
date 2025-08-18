#pragma once

#include <raylib.h>

#include <array>

// Physical elements
static constexpr float OBSTACLE_RADIUS = 80.0f;
static constexpr float DEVIATION_DISTANCE_MAX = 0.8f * OBSTACLE_RADIUS;
static constexpr float START_RADIUS = 20.0f;
static constexpr float GOAL_RADIUS = 30.0f;
static constexpr float CHEAP_PARENT_SEARCH_RADIUS = 1.5f * GOAL_RADIUS;
static constexpr float RADIUS_OF_CURVATURE_MIN = 0.95f * std::min(OBSTACLE_RADIUS, 2.0f * GOAL_RADIUS);
static constexpr float GOAL_SAMPLE_PROBABILITY = 0.02;

static constexpr float OBSTACLE_SPACING_MIN = 10.0f;
static constexpr float OBSTACLE_DEL_RADIUS = 20.0f;

static constexpr int GRID_THICKNESS = 3;
static constexpr int GRID_SPACING = 100;

static constexpr Vector2 DEFAULT_START = {100, 500};
static constexpr Vector2 DEFAULT_GOAL = {650, 500};

const std::vector<Vector2> DEFAULT_OBSTACLES = {
    {440, 1000},
    {440, 980},
    {440, 960},
    {440, 940},
    {440, 920},
    {440, 900},
    {440, 880},
    {440, 860},
    {440, 840},
    {440, 820},
    {440, 800},
    {440, 780},
    {440, 760},
    {440, 740},
    {440, 720},
    {440, 700},
    {440, 680},
    {440, 660},
    {440, 640},
    {440, 620},
    {440, 600},
    {440, 580},
    {440, 560},
    {440, 540},
    {440, 520},
    {440, 480},
    {440, 440},
    {440, 420},
    {440, 400},
    {440, 380},
    {440, 360},
    {440, 340},
    {440, 320},
    {440, 300},
    {440, 280},
    {440, 260},
    {440, 240},
    {440, 220},
    {440, 200},
    {440, 180},
    {460, 180},
    {480, 180},
    {500, 180},
    {520, 180},
    {540, 180},
    {560, 180},
    {580, 180},
    {600, 180},
    {620, 180},
    {640, 180},
    {660, 180},
    {680, 180},
    {700, 180},
    {720, 180},
    {740, 180},
    {760, 180},
    {780, 180},
    {800, 180},
    {820, 180},
    {840, 180},
    {860, 180},
    {860, 220},
    {860, 240},
    {860, 260},
    {860, 280},
    {860, 300},
    {860, 320},
    {860, 340},
    {860, 360},
    {860, 380},
    {860, 400},
    {860, 420},
    {860, 440},
    {860, 460},
    {860, 480},
    {860, 500},
    {860, 520},
    {860, 540},
    {860, 560},
    {860, 580},
    {860, 600},
    {860, 620},
    {860, 640},
    {860, 660},
    {860, 680},
    {860, 700},
    {860, 720},
    {860, 740},
    {860, 760},
    {860, 780},
    {860, 800},
    {860, 820},
    {1240, 40},
    {1240, 60},
    {1240, 80},
    {1240, 100},
    {1240, 120},
    {1240, 140},
    {1240, 160},
    {1240, 180},
    {1240, 200},
    {1240, 220},
    {1240, 240},
    {1240, 260},
    {1240, 280},
    {1240, 300},
    {1240, 320},
    {1240, 340},
    {1240, 360},
    {1240, 380},
    {1240, 400},
    {1240, 420},
    {1240, 440},
    {1240, 460},
    {1240, 480},
    {1240, 500},
    {1240, 520},
    {1240, 540},
    {1240, 560},
    {1240, 580},
    {1240, 600},
    {1240, 620},
    {1240, 640},
    {1240, 660},
    {1240, 680},
    {1240, 700},
    {1240, 720},
    {1240, 740},
    {1240, 760},
    {1240, 780},
    {1240, 800},
    {1240, 820},
    {1260, 820},
    {1280, 820},
    {1300, 820},
    {1320, 820},
    {1340, 820},
    {1360, 820},
    {1380, 820},
    {1400, 820},
    {1420, 820},
    {1440, 820},
    {1460, 820},
    {1480, 820},
    {1500, 820},
    {1520, 820},
    {1540, 820},
    {1560, 820},
    {1580, 820},
    {1600, 820},
    {1620, 820},
    {1620, 800},
    {1620, 780},
    {1620, 760},
    {1620, 740},
    {1620, 720},
    {1620, 700},
    {1620, 680},
    {1620, 660},
    {1620, 640},
    {1620, 620},
    {1620, 600},
    {1620, 580},
    {1620, 560},
    {1620, 540},
    {1620, 520},
    {1620, 500},
    {1620, 480},
    {1620, 460},
    {1620, 440},
    {1620, 420},
    {1620, 400},
    {1620, 380},
    {1620, 360},
    {1620, 340},
    {1620, 320},
    {1620, 300},
    {1620, 280},
    {1620, 260},
    {1620, 240},
    {1620, 220},
    {1620, 200}};

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
static constexpr int TEXT_HEIGHT_STAT = 0.3 * RIBBON_ROW_HEIGHT;

static constexpr int LINE_WIDTH_TREE = 5;
static constexpr int LINE_WIDTH_PATH = 15;
static constexpr int NODE_WIDTH_PATH = 30;
