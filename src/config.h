#pragma once

#include <raylib.h>

#include <array>

// UI OPTIONS
static constexpr std::array<int, 10> NUM_SAMPLES_OPTIONS = {0, 1, 10, 100, 200, 500, 1000, 2000, 5000, 10000};
static constexpr std::array<int, 11> NUM_CARRY_OPTIONS = {0, 1, 10, 100, 200, 500, 1000, 2000, 5000, 10000, 20000};

// UI SIZES
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

static constexpr int BUTTON_SPACING_X = 16;
static constexpr int BUTTON_SPACING_Y = 16;

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
static constexpr int SMALL_BUTTON_ICON_SCALE = 3;
static constexpr int VISIBILITY_BUTTON_ICON_SCALE = 3;

// UI TIMES
static constexpr float MOMENT_DURATION = 0.100f;

// TIMING
static constexpr float TIMING_WINDOW_SEC = 1.0f;

// PHYSICAL ELEMENTS
static constexpr float OBSTACLE_RADIUS = 0.8f * CELL_SIZE;
static constexpr float OBSTACLE_RADIUS_SQR = OBSTACLE_RADIUS * OBSTACLE_RADIUS;

static constexpr float GOAL_RADIUS = CELL_SIZE / 2;
static constexpr float START_RADIUS = GOAL_RADIUS;

static constexpr float OBSTACLE_SPACING_MIN = 0.25f * CELL_SIZE;
static constexpr float OBSTACLE_DELETE_RADIUS = 10.0f;

static constexpr float START_CHANGED_DIST_MIN = 1.0f;
static constexpr float START_CHANGED_DIST_MIN_SQR = START_CHANGED_DIST_MIN * START_CHANGED_DIST_MIN;

static constexpr Vector2 DEFAULT_START = {630, 270};
static constexpr Vector2 DEFAULT_GOAL = {1290, 270};

const std::vector<Vector2> DEFAULT_OBSTACLES = {
    {1530, 510},
    {1470, 510},
    {1410, 510},
    {1350, 510},
    {1290, 510},
    {1230, 510},
    {1170, 510},
    {1110, 510},
    {1110, 450},
    {1110, 390},
    {1110, 330},
    {1110, 270},
    {1110, 210},
    {1110, 150},
    {1170, 150},
    {1230, 150},
    {1290, 150},
    {1350, 150},
    {1410, 150},
    {1410, 210},
    {1410, 270},
    {1410, 330},
    {1410, 390},
    {1350, 390},
    {1290, 390},
    {1230, 390},
    {1530, 570},
    {1470, 570},
    {1410, 570},
    {1350, 570},
    {1290, 570},
    {1230, 570},
    {1170, 570},
    {1110, 570},
    {1110, 630},
    {1110, 690},
    {1110, 750},
    {1110, 810},
    {1110, 870},
    {1110, 930},
    {1170, 930},
    {1230, 930},
    {1290, 930},
    {1350, 930},
    {1410, 930},
    {1410, 870},
    {1410, 810},
    {1410, 750},
    {1410, 690},
    {1350, 690},
    {1290, 690},
    {1230, 690},
    {810, 30},
    {810, 90},
    {810, 150},
    {810, 210},
    {810, 270},
    {810, 330},
    {810, 390},
    {810, 690},
    {810, 750},
    {810, 810},
    {810, 870},
    {810, 930},
    {810, 990},
    {810, 1050},
    {510, 450},
    {510, 510},
    {510, 570},
    {510, 630},
    {510, 690},
    {510, 750},
    {510, 810},
    {510, 870},
    {510, 930},
    {750, 390},
    {690, 390},
    {630, 390},
    {570, 390},
    {510, 390},
};

// PLANNER
static constexpr float GOAL_SAMPLE_PROBABILITY = 0.02;

static constexpr float MAX_DISTANCE_BETWEEN_POSES_FOR_COLLISION_CHECK = 0.5f * OBSTACLE_RADIUS;
static constexpr int NUM_INTERMEDIATE_COLLISION_CHECK_POINTS = 16;
static constexpr float DEVIATION_DISTANCE_MAX = NUM_INTERMEDIATE_COLLISION_CHECK_POINTS * MAX_DISTANCE_BETWEEN_POSES_FOR_COLLISION_CHECK;

static constexpr float CHEAP_PARENT_SEARCH_RADIUS = 1.5f * GOAL_RADIUS;

static constexpr float REWIRE_RADIUS = std::min(DEVIATION_DISTANCE_MAX, CHEAP_PARENT_SEARCH_RADIUS);

static constexpr float RADIUS_OF_CURVATURE_MIN = 0.95f * std::min(OBSTACLE_RADIUS, 2.0f * GOAL_RADIUS);
