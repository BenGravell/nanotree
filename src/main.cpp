#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "config.h"
#include "draw.h"
#include "mode.h"
#include "obstacle.h"
#include "rng.h"
#include "tree.h"

// TODO these rectangles should not be globals...

const Rectangle place_goal_button = {0 + 0.0 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
const Rectangle del_obstacle_button = {0 + 1.0 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};
const Rectangle add_obstacle_button = {0 + 1.5 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};
const Rectangle dec_tree_size_button = {0 + 2.0 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};
const Rectangle inc_tree_size_button = {0 + 2.5 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};
const Rectangle dec_num_samples_button = {0 + 3.0 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};
const Rectangle inc_num_samples_button = {0 + 3.5 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};

const Rectangle ribbon_row_2_col_0 = {0 + 0 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
const Rectangle ribbon_row_2_col_1 = {0 + 1 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
const Rectangle ribbon_row_2_col_2 = {0 + 2 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
const Rectangle ribbon_row_2_col_3 = {0 + 3 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};

const std ::vector<Rectangle> ribbon_rectangles = {place_goal_button,
                                                   del_obstacle_button,
                                                   add_obstacle_button,
                                                   dec_tree_size_button,
                                                   inc_tree_size_button,
                                                   dec_num_samples_button,
                                                   inc_num_samples_button,
                                                   ribbon_row_2_col_0,
                                                   ribbon_row_2_col_1,
                                                   ribbon_row_2_col_2,
                                                   ribbon_row_2_col_3};

std::optional<SelectorMode> getSelectorMode() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return std::nullopt;
    }

    const Vector2 mouse = GetMousePosition();
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

std::optional<int> getIdxDelta(const Rectangle& dec_button, const Rectangle& inc_button) {
    const Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, inc_button)) {
            return 1;
        }
        if (CheckCollisionPointRec(mouse, dec_button)) {
            return -1;
        }
    }
    return std::nullopt;
}

int getScrollSign() {
    const int scroll = GetMouseWheelMove();
    return (scroll > 0) - (scroll < 0);
}

int getSamplesIdxDelta() {
    return getIdxDelta(dec_num_samples_button, inc_num_samples_button).value_or(getScrollSign());
}

int getTreeTargetMaxSizeIdxDelta() {
    return getIdxDelta(dec_tree_size_button, inc_tree_size_button).value_or(0);
}

int updateSamplesOption(const int num_samples, const int idx_delta) {
    const int idx_old = int(std::lower_bound(NUM_SAMPLES_OPTIONS.begin(), NUM_SAMPLES_OPTIONS.end(), num_samples) - NUM_SAMPLES_OPTIONS.begin());
    const int idx_new = std::clamp(idx_old + idx_delta, 0, int(NUM_SAMPLES_OPTIONS.size() - 1));
    return NUM_SAMPLES_OPTIONS[idx_new];
}

int main() {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "nanotree");

    Font font = LoadFontEx("assets/Oxanium-Regular.ttf", 40, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    Vector2 goal = DEFAULT_GOAL;
    Obstacles obstacles = DEFAULT_OBSTACLES;
    Tree tree;
    tree.reset();
    Path path;

    int num_samples = 500;
    int tree_target_max_size = 5000;
    SelectorMode mode = SelectorMode::PLACE_GOAL;
    bool show_debug = false;
    float dt_draw = 0;
    bool first_iter = true;

    // pre-growth
    for (int i = 0; i < tree_target_max_size / num_samples; ++i) {
        tree.grow(num_samples, goal, obstacles);
    }

    while (!WindowShouldClose()) {
        // ---- UI LOGIC
        Vector2 mouse = GetMousePosition();

        const bool is_down_lmb = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        const bool is_down_rmb = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
        const bool is_down_mmb = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);

        mode = getSelectorMode().value_or(mode);
        num_samples = updateSamplesOption(num_samples, getSamplesIdxDelta());
        tree_target_max_size = updateSamplesOption(tree_target_max_size, getTreeTargetMaxSizeIdxDelta());

        const Vector2 selector_pos = clampToEnvironment(mouse);
        const bool mouse_in_environment = insideEnvironment(mouse);
        bool tree_should_reset = false;
        bool tree_should_grow = false;
        if (mouse_in_environment) {
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

        if (IsKeyPressed(KEY_G)) {
            tree_should_grow = true;
        }
        if (IsKeyDown(KEY_T)) {
            tree_should_grow = true;
        }

        if (IsKeyPressed(KEY_R)) {
            tree_should_reset = true;
        }

        if (IsKeyPressed(KEY_D)) {
            show_debug = !show_debug;
        }

        // ---- PLANNER LOGIC

        if (tree_should_reset) {
            tree.reset();
        }

        float t1_retain_tree = GetTime();
        float t2_retain_tree = GetTime();
        float t1_grow_tree = GetTime();
        float t2_grow_tree = GetTime();
        if (tree_should_grow) {
            t1_retain_tree = GetTime();
            tree.retain(path, tree_target_max_size);
            t2_retain_tree = GetTime();

            t1_grow_tree = GetTime();
            tree.grow(num_samples, goal, obstacles);
            t2_grow_tree = GetTime();
        }
        const float dt_retain_tree = t2_retain_tree - t1_retain_tree;
        const float dt_grow_tree = t2_grow_tree - t1_grow_tree;

        const float t1_extract_path = GetTime();
        path = extractPath(goal, tree.nodes);
        const float t2_extract_path = GetTime();
        const float dt_extract_path = t2_extract_path - t1_extract_path;

        const bool goal_reached = Vector2Distance(path.back()->pos, goal) < GOAL_REACHED_RADIUS;

        const int fps = GetFPS();

        // ---- DRAWING
        const float t1_draw = GetTime();
        BeginDrawing();
        DrawRectangle(0, 0, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT, COLOR_BACKGROUND);
        DrawRectangleLines(0, 0, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT, COLOR_TEXT_CONTROL_SELECT_BKGD);
        DrawObstacles(obstacles);
        DrawTree(tree, path);
        DrawPath(path);
        DrawSelectorByMode(selector_pos, mode);
        DrawGoal(goal, goal_reached);
        DrawRibbon(tree, num_samples, tree_target_max_size, goal_reached, mode, fps, ribbon_rectangles, font);

        if (show_debug) {
            DrawRectangle(0, 0, 500, 220, COLOR_RIBBON_BACKGROUND);
            DrawRectangleLines(0, 0, 500, 220, COLOR_TEXT_CONTROL_SELECT_BKGD);
            DrawTextEx(font, TextFormat("%4d ms [retain tree]", int(1000.0f * dt_retain_tree)), {10, 10 + 0 * 50}, TEXT_HEIGHT_STAT, 1, GOLD);
            DrawTextEx(font, TextFormat("%4d ms [grow tree]", int(1000.0f * dt_grow_tree)), {10, 10 + 1 * 50}, TEXT_HEIGHT_STAT, 1, GOLD);
            DrawTextEx(font, TextFormat("%4d ms [extract path]", int(1000.0f * dt_extract_path)), {10, 10 + 2 * 50}, TEXT_HEIGHT_STAT, 1, GOLD);
            DrawTextEx(font, TextFormat("%4d ms [draw]", int(1000.0f * dt_draw)), {10, 10 + 3 * 50}, TEXT_HEIGHT_STAT, 1, GREEN);
        }

        EndDrawing();
        const float t2_draw = GetTime();
        dt_draw = t2_draw - t1_draw;
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}
