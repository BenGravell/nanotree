#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "core/config.h"
#include "core/obstacle.h"
#include "core/rng.h"
#include "planner/tree.h"
#include "ui/draw.h"
#include "ui/mode.h"
#include "ui/number_widget.h"

// TODO these rectangles should not be globals... they should be collected in a Selector struct along with getSelectorMode()

const Rectangle place_start_button = {0 + 0.0 * RIBBON_COL_WIDTH + 10, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 10, RIBBON_COL_WIDTH / 2 - 20, RIBBON_ROW_HEIGHT - 20};
const Rectangle place_goal_button = {0 + 0.5 * RIBBON_COL_WIDTH + 10, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 10, RIBBON_COL_WIDTH / 2 - 20, RIBBON_ROW_HEIGHT - 20};
const Rectangle del_obstacle_button = {0 + 1.0 * RIBBON_COL_WIDTH + 10, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 10, RIBBON_COL_WIDTH / 2 - 20, RIBBON_ROW_HEIGHT - 20};
const Rectangle add_obstacle_button = {0 + 1.5 * RIBBON_COL_WIDTH + 10, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 10, RIBBON_COL_WIDTH / 2 - 20, RIBBON_ROW_HEIGHT - 20};

const std ::vector<Rectangle> ribbon_rectangles = {place_start_button,
                                                   place_goal_button,
                                                   del_obstacle_button,
                                                   add_obstacle_button};

std::optional<SelectorMode> getSelectorMode() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return std::nullopt;
    }

    const Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, place_start_button)) {
        return SelectorMode::PLACE_START;
    }
    if (CheckCollisionPointRec(mouse, place_goal_button)) {
        return SelectorMode::PLACE_GOAL;
    }
    if (CheckCollisionPointRec(mouse, add_obstacle_button)) {
        return SelectorMode::ADD_OBSTACLE;
    }
    if (CheckCollisionPointRec(mouse, del_obstacle_button)) {
        return SelectorMode::DEL_OBSTACLE;
    }
    return std::nullopt;
}

NumberWidget num_samples_widget{200, std::vector(std::begin(NUM_SAMPLES_OPTIONS), std::end(NUM_SAMPLES_OPTIONS)), true, {10 + 2 * RIBBON_COL_WIDTH, 10 + ENVIRONMENT_HEIGHT}};
NumberWidget num_carryover_widget{2000, std::vector(std::begin(NUM_CARRYOVER_OPTIONS), std::end(NUM_CARRYOVER_OPTIONS)), false, {10 + 3 * RIBBON_COL_WIDTH, 10 + ENVIRONMENT_HEIGHT}};

int main() {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "nanotree");

    Font font = LoadFontEx("assets/Oxanium/static/Oxanium-Regular.ttf", 40, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    Vector2 start = DEFAULT_START;
    Vector2 goal = DEFAULT_GOAL;
    Obstacles obstacles = DEFAULT_OBSTACLES;

    Tree tree;
    tree.reset(start);

    Path path;

    int num_samples = 200;
    int num_carryover = 2000;
    SelectorMode mode = SelectorMode::PLACE_GOAL;
    bool show_debug = false;
    bool carryover_path = true;
    float dt_draw = 0;
    bool first_iter = true;

    // Pre-grow tree to fill it up to carryover limit.
    for (int i = 0; i < num_carryover / num_samples; ++i) {
        tree.grow(num_samples, goal, obstacles);
    }

    while (!WindowShouldClose()) {
        // ---- UI LOGIC
        Vector2 mouse = GetMousePosition();

        const bool is_down_lmb = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        const bool is_down_rmb = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
        const bool is_down_mmb = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);

        mode = getSelectorMode().value_or(mode);

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

        tree_should_grow = tree_should_grow || first_iter;
        first_iter = false;

        // KEYMAP
        // MMB: Delete obstacle
        // RMB: Add obstacle
        // Scroll: Adjust num samples
        // G: Grow tree (once)
        // T: Grow tree (continuously)
        // P: Toggle carryover of path
        // R: Reset tree
        // D: Show debug info

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

        if (IsKeyPressed(KEY_D)) {
            show_debug = !show_debug;
        }

        // ---- PLANNER LOGIC

        if (tree_should_reset) {
            tree.reset(start);
        }

        float t1_carryover = GetTime();
        float t2_carryover = GetTime();
        float t1_grow_tree = GetTime();
        float t2_grow_tree = GetTime();
        if (tree_should_grow) {
            t1_carryover = GetTime();
            tree.carryover(path, num_carryover, carryover_path);
            t2_carryover = GetTime();

            t1_grow_tree = GetTime();
            tree.grow(num_samples, goal, obstacles);
            t2_grow_tree = GetTime();
        }
        const float dt_carryover = t2_carryover - t1_carryover;
        const float dt_grow_tree = t2_grow_tree - t1_grow_tree;

        const float t1_extract_path = GetTime();
        path = extractPath(goal, tree.nodes);
        const float t2_extract_path = GetTime();
        const float dt_extract_path = t2_extract_path - t1_extract_path;

        const bool goal_reached = Vector2Distance(path.back()->pos, goal) < GOAL_RADIUS;

        const int fps = GetFPS();

        // ---- DRAWING LOGIC
        const float t1_draw = GetTime();
        BeginDrawing();
        DrawRectangle(0, 0, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT, COLOR_BACKGROUND);
        DrawFlatGrid(0, ENVIRONMENT_WIDTH, 0, ENVIRONMENT_HEIGHT, {GRID_SPACING_MINOR, GRID_THICKNESS_MINOR, COLOR_GRID_MINOR});
        DrawFlatGrid(0, ENVIRONMENT_WIDTH, 0, ENVIRONMENT_HEIGHT, {GRID_SPACING_MAJOR, GRID_THICKNESS_MAJOR, COLOR_GRID_MAJOR});
        DrawObstacles(obstacles);
        DrawTree(tree, path, goal);
        DrawPath(path);
        DrawSelectorByMode(selector_pos, mode);
        DrawStart(start);
        DrawGoal(goal, goal_reached);
        DrawRibbon(tree, num_samples, num_carryover, goal_reached, mode, fps, ribbon_rectangles, font);
        DrawNumberWidget(num_samples_widget);
        DrawNumberWidget(num_carryover_widget);

        if (show_debug) {
            DrawRectangle(0, 0, 500, 220, COLOR_RIBBON_BACKGROUND);
            DrawRectangleLines(0, 0, 500, 220, COLOR_TEXT_CONTROL_SELECT_BKGD);
            DrawTextEx(font, TextFormat("%4d ms [carryover]", int(1000.0f * dt_carryover)), {10, 10 + 0 * 50}, TEXT_HEIGHT_STAT, 1, GOLD);
            DrawTextEx(font, TextFormat("%4d ms [grow tree]", int(1000.0f * dt_grow_tree)), {10, 10 + 1 * 50}, TEXT_HEIGHT_STAT, 1, GOLD);
            DrawTextEx(font, TextFormat("%4d ms [extract path]", int(1000.0f * dt_extract_path)), {10, 10 + 2 * 50}, TEXT_HEIGHT_STAT, 1, GOLD);
            DrawTextEx(font, TextFormat("%4d ms [draw]", int(1000.0f * dt_draw)), {10, 10 + 3 * 50}, TEXT_HEIGHT_STAT, 1, GREEN);
        }

        DrawRectangleLinesEx({0, 0, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT}, 3, COLOR_TEXT_CONTROL_SELECT_BKGD);

        EndDrawing();
        const float t2_draw = GetTime();
        dt_draw = t2_draw - t1_draw;
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}
