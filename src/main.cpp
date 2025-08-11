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
    int mode = 1;

    const Rectangle place_goal_button = {0 + 0 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle add_obstacle_button = {0 + 1 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle del_obstacle_button = {0 + 2 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT};
    const Rectangle dec_num_samples_button = {0 + 3 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};
    const Rectangle inc_num_samples_button = {0 + 3.5 * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT, RIBBON_COL_WIDTH / 2, RIBBON_ROW_HEIGHT};

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

        const bool mouse_in_environment = insideEnvironment(mouse);
        mouse = clampToEnvironment(mouse);

        bool problem_changed = false;

        if (mouse_in_environment) {
            if (is_down_lmb && mode == 1) {
                goal = mouse;
                problem_changed = true;
            }

            if ((is_down_lmb && mode == 2) || is_down_rmb) {
                if (std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o) { return Vector2Distance(o, mouse) < OBSTACLE_SPACING_MIN; })) {
                    obstacles.push_back(mouse);
                }
                problem_changed = true;
            }

            if ((is_down_lmb && mode == 3) || is_down_mmb) {
                obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, mouse) < (OBSTACLE_RADIUS + OBSTACLE_DEL_RADIUS); }), obstacles.end());
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
        DrawSelectorByMode(mouse, mode);

        // TODO func DrawGoal
        const Color goal_color = goal_reached ? COLOR_GOAL_REACHED : COLOR_GOAL_NOT_REACHED;
        DrawCircleV(goal, GOAL_REACHED_RADIUS, Fade(goal_color, 0.8f));

        // TODO func DrawRibbon
        DrawRectangle(0, ENVIRONMENT_HEIGHT, ENVIRONMENT_WIDTH, RIBBON_HEIGHT, COLOR_RIBBON_BACKGROUND);

        // TODO
        DrawRectangleLinesEx(place_goal_button, 1, COLOR_KEYMAP);
        DrawRectangleLinesEx(add_obstacle_button, 1, COLOR_KEYMAP);
        DrawRectangleLinesEx(del_obstacle_button, 1, COLOR_KEYMAP);
        DrawRectangleLinesEx(inc_num_samples_button, 1, COLOR_KEYMAP);
        DrawRectangleLinesEx(dec_num_samples_button, 1, COLOR_KEYMAP);

        for (int y = ENVIRONMENT_HEIGHT + RIBBON_ROW_HEIGHT; y < SCREEN_HEIGHT; y += RIBBON_ROW_HEIGHT) {
            for (int x = 0; x < ENVIRONMENT_WIDTH; x += RIBBON_COL_WIDTH) {
                DrawRectangleLines(x, y, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT, COLOR_KEYMAP);
            }
        }

        // TODO use the Rectangle objects directly
        // TODO make class for clickable button to handle color of button and text
        for (int m = 1; m <= 3; ++m) {
            if (m == mode) {
                int x = (m - 1) * RIBBON_COL_WIDTH;
                int y = ENVIRONMENT_HEIGHT;
                DrawRectangle(x, y, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT, COLOR_KEYMAP);
            }
        }

        static constexpr int TEXT_MARGIN_WIDTH = 20;

        static constexpr int RIBBON_ROW_1A_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 15;
        static constexpr int RIBBON_ROW_1B_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 60;
        static constexpr int RIBBON_ROW_2_Y = ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;

        // TODO center all text
        DrawText(std::string(goal_reached ? "Goal reached" : "Goal missed").c_str(), 0 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT_STAT, goal_color);
        DrawText(TextFormat("%2i FPS", fps), 1 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT_STAT, fpsColor(fps));
        DrawText((std::to_string(tree.nodes.size()) + " nodes").c_str(), 2 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT_STAT, COLOR_NODE_COUNT);
        DrawText((std::to_string(num_samples) + " samples").c_str(), 3 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT_STAT, COLOR_NODE_COUNT);

        DrawText("Place goal", 0 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, (mode == 1) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
        DrawText("Insert obstacle", 1 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, (mode == 2) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
        DrawText("Delete obstacle", 2 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, (mode == 3) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
        DrawText("- samples", 3.0 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, COLOR_KEYMAP);
        DrawText("+ samples", 3.5 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, COLOR_KEYMAP);

        DrawText("[LMB] to engage", 0 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, (mode == 1) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
        DrawText("[LMB] to engage / [RMB]", 1 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, (mode == 2) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
        DrawText("[LMB] to engage / [MMB]", 2 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, (mode == 3) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
        DrawText("[LMB] here / [Scroll]", 3.0 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, COLOR_KEYMAP);
        DrawText("[LMB] here / [Scroll]", 3.5 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, COLOR_KEYMAP);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
