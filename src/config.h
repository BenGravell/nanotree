#include <raylib.h>

static constexpr int OBSTACLE_RADIUS = 100;
static constexpr float DEVIATION_DISTANCE_MAX = 0.6f * OBSTACLE_RADIUS;
static constexpr float RADIUS_OF_CURVATURE_MIN = 0.9f * OBSTACLE_RADIUS;
static constexpr float GOAL_REACHED_RADIUS = 40.0f;
static constexpr float GOAL_SAMPLE_PROBABILITY = 0.02;

static constexpr int OBSTACLE_SPACING_MIN = 10;
static constexpr int OBSTACLE_DEL_RADIUS = 20;

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
static constexpr int TEXT_HEIGHT_CONTROL = 0.2 * RIBBON_ROW_HEIGHT;
static constexpr int TEXT_HEIGHT_STAT = 0.4 * RIBBON_ROW_HEIGHT;

static constexpr int LINE_WIDTH_PATH = 12;
static constexpr int LINE_WIDTH_TREE = 4;
static constexpr int NODE_WIDTH_PATH = 30;

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