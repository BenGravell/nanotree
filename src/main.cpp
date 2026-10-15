#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "core/mouse_state.h"
#include "core/problem/problem_editor.h"
#include "core/timing/timing_parts.h"
#include "planner/planner.h"
#include "ui/drawing/ctrl_bar.h"
#include "ui/drawing/environment.h"
#include "ui/drawing/screen_border.h"
#include "ui/drawing/stat_bar.h"
#include "ui/style.h"

int main() {
  // RAYLIB INIT
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "nanotree");

  const Font font = loadGuiFont();
  initializeGuiStyle(font);

  // PLAN SETTINGS INIT
  CtrlState ctrl_state;
  const PlanSettings plan_settings = getPlanSettings(ctrl_state);

  // TIMING INIT
  AppTimingParts app_timing;

  // ENVIRONMENT INIT
  Problem problem = {DEFAULT_OBSTACLES, DEFAULT_START, DEFAULT_GOAL};

  // PLANNER INIT
  Planner planner;
  planner.prep(problem, plan_settings);

  // PROBLEM EDITOR INIT
  ProblemEditor editor;

  // CORE APP LOOP
  while (!WindowShouldClose()) {
    app_timing.total.start();

    const MouseState mouse = getMouseState(ctrl_state.snap_to_grid);
    const ProblemEdits problem_edits = editor.update(problem, ctrl_state, mouse);
    const PlanSettings plan_settings = getPlanSettings(ctrl_state);

    // PLANNING
    const ActionSettings action_settings = {problem_edits, ctrl_state.tree_edits};

    planner.plan(problem, plan_settings, action_settings);

    const bool goal_reached = goalReached(planner.path, problem.goal);
    const DurationParts duration = averageDurations(planner.timing, app_timing);
    const TreeStats tree_stats = computeTreeStats(planner.tree.nodes, planner.path, problem.goal);

    // DRAWING
    app_timing.draw.start();
    BeginDrawing();

    DrawEnvironment(problem, planner, tree_stats, mouse.pos, ctrl_state, goal_reached);
    DrawStatBar(problem, tree_stats, goal_reached, duration);
    DrawCtrlBar(ctrl_state, goal_reached);
    DrawScreenBorder();

    EndDrawing();
    app_timing.draw.record();
    app_timing.total.record();
  }
  UnloadFont(font);
  CloseWindow();
  return 0;
}
