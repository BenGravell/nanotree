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

void plan(Tree& tree, Path& path, const Vector2 start, const Vector2 goal, const Obstacles& obstacles, const int num_carry, const int num_samples, const bool rewire_enabled) {
    tree.grow(num_samples, goal, obstacles, rewire_enabled);
    tree.carry(path, num_carry, obstacles);
    path = extractPath(goal, tree.nodes, obstacles);
}

void prepTree(Tree& tree, Path& path, const Vector2 start, const Vector2 goal, const Obstacles& obstacles, const int num_carry, const int num_samples, const bool rewire_enabled) {
    tree.reset(start);

    // Run the planner until:
    // 1: tree filled up to carry limit
    // 2: goal reached
    // 3: ran out of attempts
    static constexpr int num_init_attempts_scale = 5;
    const int num_init_attempts_max = num_init_attempts_scale * (num_carry / num_samples);
    int num_init_attempts = 0;
    {
        bool goal_reached = false;
        while ((tree.nodes.size() < num_carry) || !goal_reached) {
            plan(tree, path, start, goal, obstacles, num_carry, num_samples, rewire_enabled);
            goal_reached = goalReached(path, goal);
            num_init_attempts++;
            if (num_init_attempts >= num_init_attempts_max) {
                break;
            }
        }
    }
}

int main() {
    // RAYLIB INIT
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "nanotree");

    // GUI STYLE INIT
    Font font = LoadFontEx("assets/Bai_Jamjuree/BaiJamjuree-Regular.ttf", BIG_TEXT_HEIGHT, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    GuiSetFont(font);

    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);
    GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, std::lround(0.9 * TEXT_HEIGHT));

    GuiSetIconScale(BUTTON_ICON_SCALE);

    GuiSetStyle(DEFAULT, BORDER_WIDTH, BORDER_THICKNESS);
    GuiSetStyle(TOGGLE, GROUP_PADDING, BUTTON_SPACING_Y);

    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(COLOR_GRAY_096));
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(COLOR_GRAY_064));
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(COLOR_GRAY_160));

    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, ColorToInt(COLOR_GRAY_160));
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(COLOR_GRAY_128));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(COLOR_LITE));

    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, ColorToInt(COLOR_LITE));
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, ColorToInt(COLOR_LITE));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(COLOR_GRAY_064));

    GuiSetStyle(DEFAULT, BORDER_COLOR_DISABLED, ColorToInt(COLOR_GRAY_064));
    GuiSetStyle(DEFAULT, BASE_COLOR_DISABLED, ColorToInt(COLOR_GRAY_048));
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, ColorToInt(COLOR_GRAY_096));

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);

    // GUI ELEMENTS INIT
    CtrlState ctrl_state;
    const int num_carry = NUM_CARRY_OPTIONS[ctrl_state.num_carry_ix];
    const int num_samples = NUM_SAMPLES_OPTIONS[ctrl_state.num_samples_ix];
    const bool rewire_enabled = ctrl_state.rewire_enabled;

    TimingParts timing;

    // ENVIRONMENT INIT
    Vector2 start = DEFAULT_START;
    Vector2 goal = DEFAULT_GOAL;
    Obstacles obstacles = DEFAULT_OBSTACLES;
    bool start_changed = true;
    bool obstacle_added = true;

    // PLANNER INIT
    Tree tree;
    Path path;
    prepTree(tree, path, start, goal, obstacles, num_carry, num_samples, rewire_enabled);

    while (!WindowShouldClose()) {
        timing.total.start();

        // ---- UI LOGIC
        Vector2 mouse = GetMousePosition();
        const bool is_down_lmb = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        const bool mouse_in_environment = insideEnvironment(mouse);
        Vector2 brush_pos = clampToEnvironment(mouse);
        const SelectorMode mode = ctrl_state.selector_mode;

        if (ctrl_state.snap_to_grid) {
            brush_pos.x = snapToGridCenter(brush_pos.x, CELL_SIZE);
            brush_pos.y = snapToGridCenter(brush_pos.y, CELL_SIZE);
        }

        // TODO refactor all distance checks to use Vector2DistanceSqr
        // TODO factor the mode action to a function and switch case on mode
        start_changed = false;
        obstacle_added = false;
        if (mouse_in_environment) {
            if (is_down_lmb && mode == SelectorMode::PLACE_START) {
                start_changed = Vector2DistanceSqr(start, brush_pos) > START_CHANGED_DIST_MIN_SQR;
                if (start_changed) {
                    start = brush_pos;
                }
            }
            if (is_down_lmb && mode == SelectorMode::PLACE_GOAL) {
                goal = brush_pos;
            }

            if (is_down_lmb && mode == SelectorMode::ADD_OBSTACLE) {
                if (std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o) { return Vector2Distance(o, brush_pos) < OBSTACLE_SPACING_MIN; })) {
                    obstacles.push_back(brush_pos);
                    obstacle_added = true;
                }
            }

            if (is_down_lmb && mode == SelectorMode::DEL_OBSTACLE) {
                obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, brush_pos) < (OBSTACLE_RADIUS + OBSTACLE_DELETE_RADIUS); }), obstacles.end());
            }
        }
        if (ctrl_state.reset_obstacles) {
            obstacles = {};
        }

        const int num_carry = NUM_CARRY_OPTIONS[ctrl_state.num_carry_ix];
        const int num_samples = NUM_SAMPLES_OPTIONS[ctrl_state.num_samples_ix];

        // ---- PLANNER LOGIC

        if (ctrl_state.tree_should_reset) {
            tree.reset(start);
        }

        if (start_changed) {
            tree.resetRoot(start, goal, path, obstacles);
        }

        timing.carry.start();
        if (ctrl_state.tree_should_grow && !ctrl_state.tree_should_reset) {
            tree.carry(path, num_carry, obstacles);
        }
        timing.carry.record();

        timing.cull.start();
        const bool do_cull = obstacle_added || start_changed;
        if (do_cull) {
            tree.cullByObstacles(obstacles);
        }
        timing.cull.record();

        timing.grow.start();
        if (ctrl_state.tree_should_grow) {
            tree.grow(num_samples, goal, obstacles, ctrl_state.rewire_enabled);
        }
        timing.grow.record();

        path = extractPath(goal, tree.nodes, obstacles);

        const bool goal_reached = goalReached(path, goal);

        const DurationParts duration = timing.averageDuration();

        // ---- DRAWING LOGIC
        timing.draw.start();
        BeginDrawing();

        DrawEnvironment(brush_pos, ctrl_state.selector_mode, start, goal, goal_reached, obstacles, tree, path, ctrl_state.visibility);
        DrawStatBar(tree, path, goal, goal_reached, obstacles, duration);
        DrawCtrlBar(ctrl_state, goal_reached);

        // Border around whole screen
        DrawRectangleLinesEx(SCREEN_REC, BORDER_THICKNESS, COLOR_SCREEN_BORDER);

        EndDrawing();
        timing.draw.record();
        timing.total.record();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}
