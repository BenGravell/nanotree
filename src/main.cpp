#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include "config.h"
#include "draw.h"
#include "obstacle.h"
#include "rng.h"
#include "tree.h"

int main() {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "nanotree");

    Vector2 goal = DEFAULT_GOAL;
    Obstacles obstacles = DEFAULT_OBSTACLES;
    Tree tree;
    Path path;

    int num_samples = 2000;

    // TODO enum class
    int mode = 1;

    const Rectangle place_goal_button = {0 + 0 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle add_obstacle_button = {0 + 1 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle del_obstacle_button = {0 + 2 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle dec_num_samples_button = {0 + 3 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};
    const Rectangle inc_num_samples_button = {0 + 3.5 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};

    const Rectangle ribbon_row_2_col_0 = {0 + 0 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle ribbon_row_2_col_1 = {0 + 1 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle ribbon_row_2_col_2 = {0 + 2 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle ribbon_row_2_col_3 = {0 + 3 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        const bool mouse_in_place_goal_button = CheckCollisionPointRec(mouse, place_goal_button);
        const bool mouse_in_add_obstacle_button = CheckCollisionPointRec(mouse, add_obstacle_button);
        const bool mouse_in_del_obstacle_button = CheckCollisionPointRec(mouse, del_obstacle_button);
        const bool mouse_in_inc_num_samples_button = CheckCollisionPointRec(mouse, inc_num_samples_button);
        const bool mouse_in_dec_num_samples_button = CheckCollisionPointRec(mouse, dec_num_samples_button);

        const bool is_down_lmb = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        const bool is_down_rmb = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
        const bool is_down_mmb = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);

        if (is_down_lmb) {
            if (mouse_in_place_goal_button) {
                mode = 1;
            }
            if (mouse_in_add_obstacle_button) {
                mode = 2;
            }
            if (mouse_in_del_obstacle_button) {
                mode = 3;
            }
        }

        int scroll = GetMouseWheelMove();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (mouse_in_inc_num_samples_button) {
                scroll = 1;
            }
            if (mouse_in_dec_num_samples_button) {
                scroll = -1;
            }
        }
        if (scroll != 0) {
            const int scroll_sign = (scroll > 0) - (scroll < 0);
            const int old_idx = int(std::lower_bound(NUM_SAMPLES_OPTIONS.begin(), NUM_SAMPLES_OPTIONS.end(), num_samples) - NUM_SAMPLES_OPTIONS.begin());
            const int new_idx = std::clamp(old_idx + scroll_sign, 0, int(NUM_SAMPLES_OPTIONS.size() - 1));
            num_samples = NUM_SAMPLES_OPTIONS[new_idx];
        }

        const Vector2 selector_pos = clampToEnvironment(mouse);
        const bool mouse_in_environment = insideEnvironment(mouse);
        bool problem_changed = false;
        if (mouse_in_environment) {
            if (is_down_lmb && mode == 1) {
                goal = selector_pos;
                problem_changed = true;
            }

            if ((is_down_lmb && mode == 2) || is_down_rmb) {
                if (std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o) { return Vector2Distance(o, selector_pos) < OBSTACLE_SPACING_MIN; })) {
                    obstacles.push_back(selector_pos);
                }
                problem_changed = true;
            }

            if ((is_down_lmb && mode == 3) || is_down_mmb) {
                obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, selector_pos) < (OBSTACLE_RADIUS + OBSTACLE_DEL_RADIUS); }), obstacles.end());
                problem_changed = true;
            }
        }

        const bool grow_new_tree = problem_changed || tree.nodes.empty();
        if (grow_new_tree) {
            tree.nodes = {std::make_shared<Node>(Node{nullptr, DEFAULT_START, 0.0f})};
            tree.grow(num_samples, goal, obstacles);
        }
        path = extractPath(goal, tree.nodes);

        const bool goal_reached = Vector2Distance(path.back()->pos, goal) < GOAL_REACHED_RADIUS;

        const int fps = GetFPS();

        // DRAWING SECTION
        BeginDrawing();
        DrawRectangle(0, 0, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT, COLOR_BACKGROUND);
        DrawObstacles(obstacles);
        DrawTree(tree, path);
        DrawPath(path);
        DrawSelectorByMode(selector_pos, mode);
        DrawGoal(goal, goal_reached);

        const std ::vector<Rectangle> ribbon_rectangles = {place_goal_button,
                                                           add_obstacle_button,
                                                           del_obstacle_button,
                                                           inc_num_samples_button,
                                                           dec_num_samples_button,
                                                           ribbon_row_2_col_0,
                                                           ribbon_row_2_col_1,
                                                           ribbon_row_2_col_2,
                                                           ribbon_row_2_col_3};
        DrawRibbon(tree,
                   num_samples,
                   goal_reached,
                   mode,
                   fps,
                   ribbon_rectangles);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
