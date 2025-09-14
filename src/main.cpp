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
#include "core/problem.h"
#include "core/rng.h"
#include "core/timing_parts.h"
#include "planner/planner.h"
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

// TODO refactor all distance checks to use Vector2DistanceSqr

// TODO make editProblem a class method or smth

// DESIGN
// - should take actions as const input
// - should mutate problem
// - should return artifact describing how problem was edited
ProblemEdits editProblem(Problem& problem, const Vector2 brush_pos, const bool is_down_lmb, const ProblemEditMode mode, const bool mouse_in_environment, const bool reset_obstacles) {
    bool start_changed = false;
    bool obstacle_added = false;
    if (mouse_in_environment && is_down_lmb) {
        switch (mode) {
            case ProblemEditMode::PLACE_START: {
                start_changed = isStartChanged(problem.start, brush_pos);
                if (start_changed) {
                    problem.start = brush_pos;
                }
                break;
            }
            case ProblemEditMode::PLACE_GOAL: {
                problem.goal = brush_pos;
                break;
            }
            case ProblemEditMode::ADD_OBSTACLE: {
                if (std::none_of(problem.obstacles.begin(), problem.obstacles.end(), [&](auto& o) { return Vector2Distance(o, brush_pos) < OBSTACLE_SPACING_MIN; })) {
                    problem.obstacles.push_back(brush_pos);
                    obstacle_added = true;
                }
                break;
            }
            case ProblemEditMode::DEL_OBSTACLE: {
                problem.obstacles.erase(std::remove_if(problem.obstacles.begin(), problem.obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, brush_pos) < (OBSTACLE_RADIUS + OBSTACLE_DELETE_RADIUS); }), problem.obstacles.end());
                break;
            }
        }
    }

    if (reset_obstacles) {
        problem.obstacles = {};
    }

    return {start_changed, obstacle_added};
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
    GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, TEXT_LINE_SPACING_HEIGHT);

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

    // PLAN SETTINGS INIT
    CtrlState ctrl_state;
    const int num_carry = NUM_CARRY_OPTIONS[ctrl_state.num_carry_ix];
    const int num_samples = NUM_SAMPLES_OPTIONS[ctrl_state.num_samples_ix];
    const bool rewire_enabled = ctrl_state.rewire_enabled;
    const PlanSettings plan_settings = {num_carry, num_samples, rewire_enabled};

    // TIMING INIT
    AppTimingParts app_timing;

    // ENVIRONMENT INIT
    Problem problem = {DEFAULT_OBSTACLES, DEFAULT_START, DEFAULT_GOAL};

    // PLANNER INIT
    Planner planner;
    planner.prep(problem, plan_settings);

    while (!WindowShouldClose()) {
        app_timing.total.start();

        // ---- UI LOGIC
        const bool is_down_lmb = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        Vector2 mouse = GetMousePosition();
        const bool mouse_in_environment = insideEnvironment(mouse);
        Vector2 brush_pos = clampToEnvironment(mouse);
        if (ctrl_state.snap_to_grid) {
            brush_pos.x = snapToGridCenter(brush_pos.x, CELL_SIZE);
            brush_pos.y = snapToGridCenter(brush_pos.y, CELL_SIZE);
        }

        const ProblemEdits problem_edits = editProblem(problem, brush_pos, is_down_lmb, ctrl_state.problem_edit_mode, mouse_in_environment, ctrl_state.reset_obstacles);

        const int num_carry = NUM_CARRY_OPTIONS[ctrl_state.num_carry_ix];
        const int num_samples = NUM_SAMPLES_OPTIONS[ctrl_state.num_samples_ix];
        const bool rewire_enabled = ctrl_state.rewire_enabled;
        const PlanSettings plan_settings = {num_carry, num_samples, rewire_enabled};

        // ---- PLANNER LOGIC
        const ActionSettings action_settings = {problem_edits, ctrl_state.tree_edits};

        planner.plan(problem, plan_settings, action_settings);

        const bool goal_reached = goalReached(planner.path, problem.goal);

        const DurationParts duration = {planner.timing.grow.averageDuration(), planner.timing.carry.averageDuration(), planner.timing.cull.averageDuration(), app_timing.draw.averageDuration(), app_timing.total.averageDuration()};

        // ---- DRAWING LOGIC
        app_timing.draw.start();
        BeginDrawing();

        DrawEnvironment(problem, planner, brush_pos, ctrl_state, goal_reached);
        DrawStatBar(problem, planner, brush_pos, ctrl_state, goal_reached, duration);
        DrawCtrlBar(ctrl_state, goal_reached);

        // Border around whole screen
        DrawRectangleLinesEx(SCREEN_REC, BORDER_THICKNESS, COLOR_SCREEN_BORDER);

        EndDrawing();
        app_timing.draw.record();
        app_timing.total.record();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}
