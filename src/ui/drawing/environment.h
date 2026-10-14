#pragma once

#include <raylib.h>

#include "config.h"
#include "core/problem/problem_edit_mode.h"
#include "ui/drawing/flat_grid.h"
#include "ui/drawing/object_brush.h"
#include "ui/drawing/obstacles.h"
#include "ui/drawing/path.h"
#include "ui/drawing/start_goal.h"
#include "ui/drawing/tree.h"

void DrawEnvironment(const Problem &problem, const Planner &planner, const TreeStats &stats, const Vector2 brush_pos,
                     const CtrlState &ctrl_state, const bool goal_reached) {
  // Background
  DrawRectangleRec(ENVIRONMENT_REC, COLOR_BACKGROUND);

  // Grid
  DrawFlatGrid(ENVIRONMENT_X_MIN,
               ENVIRONMENT_X_MAX,
               ENVIRONMENT_Y_MIN,
               ENVIRONMENT_Y_MAX,
               {GRID_SPACING, GRID_THICKNESS, COLOR_GRID});

  // Objects
  if (ctrl_state.visibility.obstacles) {
    DrawObstacles(problem.obstacles);
  }
  if (ctrl_state.visibility.tree) {
    DrawTree(planner.tree, stats, problem.goal, goal_reached);
  }
  if (ctrl_state.visibility.path) {
    DrawPath(planner.path, goal_reached);
  }
  DrawObjectBrush(brush_pos, getObjectBrushParams(ctrl_state.problem_edit_mode));
  DrawStart(problem.start);
  DrawGoal(problem.goal, goal_reached);

  // Border
  DrawRectangleLinesEx(STAT_BAR_REC, BORDER_THICKNESS, COLOR_STAT_BAR_BORDER);
}