#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/drawing/flat_grid.h"
#include "ui/drawing/object_brush.h"
#include "ui/drawing/obstacles.h"
#include "ui/drawing/path.h"
#include "ui/drawing/start_goal.h"
#include "ui/drawing/tree.h"
#include "ui/problem_edit_mode.h"

// TODO move
DrawObjectBrushParams getObjectBrushParams(const ProblemEditMode mode) {
    switch (mode) {
        case ProblemEditMode::PLACE_START:
            return {START_RADIUS, 0.4f, 8, 4.0f};
        case ProblemEditMode::PLACE_GOAL:
            return {GOAL_RADIUS, 0.4f, 8, 4.0f};
        case ProblemEditMode::ADD_OBSTACLE:
            return {OBSTACLE_RADIUS, 0.3f, 12, 5.0f};
        case ProblemEditMode::DEL_OBSTACLE:
            return {OBSTACLE_DELETE_RADIUS, 0.5f, 6, 3.0f};
        default:
            return {0, 0.0f, 0, 0.0f};
    }
}

void DrawEnvironment(const Vector2 brush_pos, const ProblemEditMode brush_mode, const Vector2 start, const Vector2 goal, const bool goal_reached, const Obstacles obstacles, const Tree& tree, const Path& path, const Visibility& visibility) {
    DrawRectangleRec(ENVIRONMENT_REC, COLOR_BACKGROUND);
    DrawFlatGrid(ENVIRONMENT_X_MIN, ENVIRONMENT_X_MAX, ENVIRONMENT_Y_MIN, ENVIRONMENT_Y_MAX, {GRID_SPACING, GRID_THICKNESS, COLOR_GRID});
    if (visibility.obstacles) {
        DrawObstacles(obstacles);
    }
    if (visibility.tree) {
        DrawTree(tree, path, goal, goal_reached);
    }
    if (visibility.path) {
        DrawPath(path, goal_reached);
    }
    DrawObjectBrush(brush_pos, getObjectBrushParams(brush_mode));
    DrawStart(start);
    DrawGoal(goal, goal_reached);
}