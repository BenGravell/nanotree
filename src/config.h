#pragma once

#include <raylib.h>

#include <array>

// Physical elements
static constexpr float OBSTACLE_RADIUS = 100.0f;
static constexpr float DEVIATION_DISTANCE_MAX = 0.6f * OBSTACLE_RADIUS;
static constexpr float GOAL_REACHED_RADIUS = 50.0f;
static constexpr float CHEAP_PARENT_SEARCH_RADIUS = 0.9f * GOAL_REACHED_RADIUS;
static constexpr float RADIUS_OF_CURVATURE_MIN = 0.95f * std::min(OBSTACLE_RADIUS, 2.0f * GOAL_REACHED_RADIUS);
static constexpr float GOAL_SAMPLE_PROBABILITY = 0.02;

static constexpr int OBSTACLE_SPACING_MIN = 10;
static constexpr int OBSTACLE_DEL_RADIUS = 20;

static constexpr Vector2 DEFAULT_START = {100, 500};
static constexpr Vector2 DEFAULT_GOAL = {1000, 500};

const std::vector<Vector2> DEFAULT_OBSTACLES = {
    {760, 260}, {760, 380}, {760, 500}, {760, 620}, {760, 740}, {880, 260}, {880, 740}, {1000, 260}, {1000, 740}, {1120, 260}, {1120, 740}, {1240, 260}, {1240, 380}, {1240, 620}, {1240, 740}};

static constexpr std::array<int, 14> NUM_SAMPLES_OPTIONS = {0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};

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
static constexpr int TEXT_HEIGHT_STAT = 0.6 * RIBBON_ROW_HEIGHT;
static constexpr int LINE_WIDTH_TREE = 8;
static constexpr int LINE_WIDTH_PATH = 20;
static constexpr int NODE_WIDTH_PATH = 40;

// Colors
static constexpr Color COLOR_BACKGROUND = {16, 16, 16, 255};
static constexpr Color COLOR_RIBBON_BACKGROUND = {32, 32, 32, 255};
static constexpr Color COLOR_OBSTACLE = {96, 96, 96, 255};

static constexpr Color COLOR_GOAL_REACHED = SKYBLUE;
static constexpr Color COLOR_GOAL_NOT_REACHED = RED;
static constexpr Color COLOR_NODE_COUNT = PURPLE;

static constexpr Color COLOR_FPS_HIGH = SKYBLUE;
static constexpr Color COLOR_FPS_MID = YELLOW;
static constexpr Color COLOR_FPS_LOW = RED;

static constexpr Color COLOR_PATH = RAYWHITE;
static constexpr Color COLOR_KEYMAP = RAYWHITE;