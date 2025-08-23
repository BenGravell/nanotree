#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "config.h"
#include "core/obstacle.h"
#include "core/rng.h"
#include "planner/tree.h"
#include "ui/drawing/flat_grid.h"
#include "ui/drawing/number_widget.h"
#include "ui/drawing/object_brush.h"
#include "ui/drawing/obstacles.h"
#include "ui/drawing/path.h"
#include "ui/drawing/ribbon.h"
#include "ui/drawing/start_goal.h"
#include "ui/drawing/statbar.h"
#include "ui/drawing/tree.h"
#include "ui/duration_parts.h"
#include "ui/elements/number_widget.h"
#include "ui/elements/selector.h"
#include "ui/timing.h"

// KEYMAP
// ------
// MMB:     Delete obstacle
// RMB:     Add obstacle
// Scroll:  Adjust assigned number setting(s)
// G:       Grow tree (once)
// T:       Grow tree (continuously)
// P:       Toggle carryover of path
// R:       Reset tree

int main() {
    // RAYLIB INIT
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "nanotree");

    // UI INIT
    Font font = LoadFontEx("assets/Oxanium/static/Oxanium-Regular.ttf", 40, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    NumberWidget num_samples_widget{200, std::vector(std::begin(NUM_SAMPLES_OPTIONS), std::end(NUM_SAMPLES_OPTIONS)), true, {10 + 2 * RIBBON_COL_WIDTH, 10 + ENVIRONMENT_HEIGHT}};
    NumberWidget num_carryover_widget{2000, std::vector(std::begin(NUM_CARRYOVER_OPTIONS), std::end(NUM_CARRYOVER_OPTIONS)), false, {10 + 3 * RIBBON_COL_WIDTH, 10 + ENVIRONMENT_HEIGHT}};

    static constexpr int BUTTON_Y = ENVIRONMENT_HEIGHT + BUTTON_MARGIN;

    Selector selector{
        .mode = SelectorMode::PLACE_GOAL,
        .rectangles = {
            {SelectorMode::PLACE_START, {0 + 0.0 * RIBBON_COL_WIDTH + BUTTON_MARGIN, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT}},
            {SelectorMode::PLACE_GOAL, {0 + 0.5 * RIBBON_COL_WIDTH + BUTTON_MARGIN, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT}},
            {SelectorMode::DEL_OBSTACLE, {0 + 1.0 * RIBBON_COL_WIDTH + BUTTON_MARGIN, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT}},
            {SelectorMode::ADD_OBSTACLE, {0 + 1.5 * RIBBON_COL_WIDTH + BUTTON_MARGIN, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT}},
        }};

    // ENVIRONMENT INIT
    Vector2 start = DEFAULT_START;
    Vector2 goal = DEFAULT_GOAL;
    Obstacles obstacles = DEFAULT_OBSTACLES;

    // PLANNER INIT
    Tree tree;
    tree.reset(start);

    Path path;

    int num_samples = num_samples_widget.current;
    int num_carryover = num_carryover_widget.current;
    bool carryover_path = true;

    // Run the planner until:
    // 1: tree filled up to carryover limit
    // 2: goal reached
    // 3: ran out of attempts
    int num_init_attempts = 0;
    const int num_init_attempts_max = 5 * (num_carryover / num_samples);
    {
        bool goal_reached = false;
        while ((tree.nodes.size() < num_carryover) || !goal_reached) {
            tree.grow(num_samples, goal, obstacles);
            tree.carryover(path, num_carryover, carryover_path);
            path = extractPath(goal, tree.nodes);
            goal_reached = Vector2Distance(path.back()->pos, goal) < GOAL_RADIUS;
            num_init_attempts++;
            if (num_init_attempts >= num_init_attempts_max) {
                break;
            }
        }
    }

    Timing timing_total;
    Timing timing_grow;
    Timing timing_carryover;
    Timing timing_draw;

    while (!WindowShouldClose()) {
        timing_total.start();
        // ---- UI LOGIC
        Vector2 mouse = GetMousePosition();

        const bool is_down_lmb = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        const bool is_down_rmb = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
        const bool is_down_mmb = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);

        selector.update();
        const SelectorMode mode = selector.mode;

        num_samples_widget.update();
        num_carryover_widget.update();
        num_samples = num_samples_widget.current;
        num_carryover = num_carryover_widget.current;

        const Vector2 selector_pos = clampToEnvironment(mouse);
        const bool mouse_in_environment = insideEnvironment(mouse);
        bool tree_should_reset = false;
        bool tree_should_grow = false;
        if (mouse_in_environment) {
            if (is_down_lmb && mode == SelectorMode::PLACE_START) {
                const bool start_changed = Vector2Distance(start, selector_pos) > START_CHANGED_DIST_MIN;
                if (start_changed) {
                    start = selector_pos;
                    tree_should_reset = true;
                } else {
                    tree_should_grow = true;
                }
            }
            if (is_down_lmb && mode == SelectorMode::PLACE_GOAL) {
                goal = selector_pos;
                tree_should_grow = true;
            }

            if ((is_down_lmb && mode == SelectorMode::ADD_OBSTACLE) || is_down_rmb) {
                if (std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o) { return Vector2Distance(o, selector_pos) < OBSTACLE_SPACING_MIN; })) {
                    obstacles.push_back(selector_pos);
                    tree_should_reset = true;
                }
            }

            if ((is_down_lmb && mode == SelectorMode::DEL_OBSTACLE) || is_down_mmb) {
                obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, selector_pos) < (OBSTACLE_RADIUS + OBSTACLE_DEL_RADIUS); }), obstacles.end());
            }
        }

        if (IsKeyPressed(KEY_G)) {
            tree_should_grow = true;
        }

        if (IsKeyDown(KEY_T)) {
            tree_should_grow = true;
        }

        if (IsKeyPressed(KEY_R)) {
            tree_should_reset = true;
        }

        if (IsKeyPressed(KEY_P)) {
            carryover_path = !carryover_path;
        }

        // ---- PLANNER LOGIC

        if (tree_should_reset) {
            tree.reset(start);
        }

        if (tree_should_grow) {
            timing_carryover.start();
            tree.carryover(path, num_carryover, carryover_path);
            timing_carryover.record();

            timing_grow.start();
            tree.grow(num_samples, goal, obstacles);
            timing_grow.record();
        } else {
            timing_carryover.start();
            timing_carryover.record();
            timing_grow.start();
            timing_grow.record();
        }

        path = extractPath(goal, tree.nodes);

        const bool goal_reached = Vector2Distance(path.back()->pos, goal) < GOAL_RADIUS;

        const float dt_grow_tree = timing_grow.averageDuration();
        const float dt_carryover = timing_carryover.averageDuration();
        const float dt_draw = timing_draw.averageDuration();
        const float dt_total = timing_total.averageDuration();
        const DurationParts duration_parts{dt_grow_tree, dt_carryover, dt_draw, dt_total};

        const int fps = std::lround(1.0f / std::max(1e-6f, dt_total));

        // ---- DRAWING LOGIC
        timing_draw.start();
        BeginDrawing();

        // Environment
        DrawRectangle(0, 0, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT, COLOR_BACKGROUND);
        DrawFlatGrid(0, ENVIRONMENT_WIDTH, 0, ENVIRONMENT_HEIGHT, {GRID_SPACING, GRID_THICKNESS, COLOR_GRID});
        DrawObstacles(obstacles);
        DrawTree(tree, path, goal);
        DrawPath(path);
        DrawObjectBrushByMode(selector_pos, mode);
        DrawStart(start);
        DrawGoal(goal, goal_reached);
        // Statbar
        DrawStatBar(tree, path, goal, goal_reached, obstacles, duration_parts, fps, font);
        // Ribbon
        DrawRibbon(num_samples_widget, num_carryover_widget, selector, font);
        // Border around whole screen
        DrawRectangleLinesEx({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, 3, COLOR_SCREEN_BORDER);

        EndDrawing();
        timing_draw.record();
        timing_total.record();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}
