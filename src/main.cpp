#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include <algorithm>
#include <memory>
#include <optional>
#include <random>
#include <vector>

#include "config.h"
#include "core/obstacle.h"
#include "core/rng.h"
#include "planner/tree.h"
#include "raygui.h"
#include "ui/drawing/ctrl_bar.h"
#include "ui/drawing/environment.h"
#include "ui/drawing/flat_grid.h"
#include "ui/drawing/object_brush.h"
#include "ui/drawing/obstacles.h"
#include "ui/drawing/path.h"
#include "ui/drawing/start_goal.h"
#include "ui/drawing/stat_bar.h"
#include "ui/drawing/tree.h"
#include "ui/timing.h"

int main() {
    // RAYLIB INIT
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "nanotree");

    // GUI STYLE INIT
    Font font = LoadFontEx("assets/Bai_Jamjuree/BaiJamjuree-Regular.ttf", TEXT_HEIGHT, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    GuiSetFont(font);
    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_WIDTH, 2*CELL_SIZE);
    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_SPACING, BUTTON_SPACING_Y);

    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);
    GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, std::lround(0.9 * TEXT_HEIGHT));

    GuiSetStyle(DEFAULT, BORDER_WIDTH, BORDER_THICKNESS);
    GuiSetStyle(TOGGLE, GROUP_PADDING, BUTTON_SPACING_Y);

    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(COLOR_GRAY_096));
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(COLOR_GRAY_064));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(COLOR_GRAY_160));

    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(COLOR_GRAY_160));
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(COLOR_GRAY_128));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(COLOR_GRAY_240));

    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(COLOR_GRAY_240));
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt(COLOR_GRAY_240));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(COLOR_GRAY_064));

    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, ColorToInt(COLOR_GRAY_064));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(COLOR_GRAY_048));
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, ColorToInt(COLOR_GRAY_096));

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);

    // GUI ELEMENTS INIT
    CtrlState ctrl_state;
    TimingParts timing;

    // ENVIRONMENT INIT
    Vector2 start = DEFAULT_START;
    Vector2 goal = DEFAULT_GOAL;
    Obstacles obstacles = DEFAULT_OBSTACLES;

    // PLANNER INIT
    Tree tree;
    tree.reset(start);

    Path path;

    const int num_carryover = NUM_CARRYOVER_OPTIONS[ctrl_state.num_carryover_ix];
    const int num_samples = NUM_SAMPLES_OPTIONS[ctrl_state.num_samples_ix];

    // TODO factor out to a free function
    // Run the planner until:
    // 1: tree filled up to carryover limit
    // 2: goal reached
    // 3: ran out of attempts
    const int num_init_attempts_max = 5 * (num_carryover / num_samples);
    int num_init_attempts = 0;
    {
        bool goal_reached = false;
        while ((tree.nodes.size() < num_carryover) || !goal_reached) {
            tree.grow(num_samples, goal, obstacles);
            tree.carryover(path, num_carryover, true);
            path = extractPath(goal, tree.nodes);
            goal_reached = Vector2Distance(path.back()->pos, goal) < GOAL_RADIUS;
            num_init_attempts++;
            if (num_init_attempts >= num_init_attempts_max) {
                break;
            }
        }
    }

    while (!WindowShouldClose()) {
        timing.total.start();

        // ---- UI LOGIC
        Vector2 mouse = GetMousePosition();
        const bool is_down_lmb = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        const bool mouse_in_environment = insideEnvironment(mouse);
        Vector2 brush_pos = clampToEnvironment(mouse);
        const SelectorMode mode = ctrl_state.selector_mode;
        bool trigger_tree_reset = false;

        if (ctrl_state.snap_to_grid) {
            int bx = std::lround(brush_pos.x);
            int by = std::lround(brush_pos.y);
            brush_pos.x = bx - bx % CELL_SIZE + CELL_SIZE / 2;
            brush_pos.y = by - by % CELL_SIZE + CELL_SIZE / 2;
            if ((ctrl_state.selector_mode == SelectorMode::ADD_OBSTACLE) || (ctrl_state.selector_mode == SelectorMode::DEL_OBSTACLE)) {
                brush_pos.x += CELL_SIZE / 2;
                brush_pos.y += CELL_SIZE / 2;
            }
        }

        // TODO factor the mode action to a function and switch case on mode
        if (mouse_in_environment) {
            if (is_down_lmb && mode == SelectorMode::PLACE_START) {
                const bool start_changed = Vector2Distance(start, brush_pos) > START_CHANGED_DIST_MIN;
                if (start_changed) {
                    start = brush_pos;
                    trigger_tree_reset = true;
                }
            }
            if (is_down_lmb && mode == SelectorMode::PLACE_GOAL) {
                goal = brush_pos;
            }

            if (is_down_lmb && mode == SelectorMode::ADD_OBSTACLE) {
                if (std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o) { return Vector2Distance(o, brush_pos) < OBSTACLE_SPACING_MIN; })) {
                    obstacles.push_back(brush_pos);
                    trigger_tree_reset = true;
                }
            }

            if (is_down_lmb && mode == SelectorMode::DEL_OBSTACLE) {
                obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, brush_pos) < (OBSTACLE_RADIUS + OBSTACLE_DEL_RADIUS); }), obstacles.end());
            }
        }

        const int num_carryover = NUM_CARRYOVER_OPTIONS[ctrl_state.num_carryover_ix];
        const int num_samples = NUM_SAMPLES_OPTIONS[ctrl_state.num_samples_ix];

        // CTRL STATE PRE-UPDATE
        if (trigger_tree_reset) {
            ctrl_state.tree_should_reset = true;
        }

        // ---- PLANNER LOGIC

        if (ctrl_state.tree_should_reset) {
            tree.reset(start);
        }

        if (ctrl_state.tree_should_grow) {
            timing.carryover.start();
            // NOTE: It is critical not to carryover the path when the tree was reset
            // to ensure path is not carried over when obstacles collide with existing path.
            // TODO: remove once carryover() method respects obstacles
            tree.carryover(path, num_carryover, ctrl_state.tree_should_reset ? false : true);
            timing.carryover.record();

            timing.grow.start();
            tree.grow(num_samples, goal, obstacles);
            timing.grow.record();
        } else {
            timing.carryover.start();
            timing.carryover.record();
            timing.grow.start();
            timing.grow.record();
        }

        path = extractPath(goal, tree.nodes);

        const bool goal_reached = Vector2Distance(path.back()->pos, goal) < GOAL_RADIUS;

        const DurationParts duration = timing.averageDuration();
        const int fps = std::lround(1.0f / std::max(1e-3f, duration.total));

        // ---- DRAWING LOGIC
        timing.draw.start();
        BeginDrawing();

        DrawEnvironment(brush_pos, ctrl_state.selector_mode, start, goal, goal_reached, obstacles, tree, path);
        DrawStatBar(tree, path, goal, goal_reached, obstacles, duration, fps);
        ctrl_state = DrawCtrlBar(ctrl_state, trigger_tree_reset, goal_reached);

        // Border around whole screen
        DrawRectangleLinesEx({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, BORDER_THICKNESS, COLOR_SCREEN_BORDER);

        EndDrawing();
        timing.draw.record();
        timing.total.record();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}
