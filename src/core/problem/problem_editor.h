#pragma once

#include <stdexcept>

#include "core/geometry.h"
#include "core/problem/problem.h"
#include "core/problem/problem_edit_mode.h"
#include "core/problem/problem_edits.h"

int brushStepCount(const Vector2 previous, const Vector2 current, const ProblemEditMode mode) {
    float spacing = 1.0f;
    if (mode == ProblemEditMode::ADD_OBSTACLE) spacing = 1.2 * OBSTACLE_SPACING_MIN;
    if (mode == ProblemEditMode::DEL_OBSTACLE) spacing = 0.5 * OBSTACLE_SPACING_MIN;
    return std::max(static_cast<int>(std::ceil(Vector2Distance(current, previous) / spacing)), 1);
}

// TODO use Vector2DistanceSqr for obstacle spacing checks.
bool addObstacles(Obstacles& obstacles, const Vector2 previous, const Vector2 current, const int steps) {
    bool added = false;
    for (int i = 1; i <= steps; ++i) {
        const Vector2 pos = Vector2Lerp(previous, current, static_cast<float>(i) / steps);
        if (std::none_of(obstacles.begin(), obstacles.end(), [&](const auto& o) {
                return Vector2Distance(o, pos) < OBSTACLE_SPACING_MIN;
            })) {
            obstacles.push_back(pos);
            added = true;
        }
    }
    return added;
}

// TODO use Vector2DistanceSqr for obstacle deletion radius checks.
void deleteObstacles(Obstacles& obstacles, const Vector2 previous, const Vector2 current, const int steps) {
    for (int i = 1; i <= steps; ++i) {
        const Vector2 pos = Vector2Lerp(previous, current, static_cast<float>(i) / steps);
        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Vector2 o) {
            return Vector2Distance(o, pos) < (OBSTACLE_RADIUS + OBSTACLE_DELETE_RADIUS);
        }), obstacles.end());
    }
}

// TODO make editProblem a class method; ProblemEditor::update currently only wraps it.
ProblemEdits editProblem(Problem& problem, const Vector2 brush_pos, const Vector2 brush_pos_prev, const bool is_down_lmb, const ProblemEditMode mode, const ProblemEditMode mode_prev, const bool mouse_in_environment, const bool reset_obstacles, const bool active_prev) {
    bool start_changed = false;
    bool obstacle_added = false;
    if (mouse_in_environment && is_down_lmb) {
        const int n = (mode == mode_prev && active_prev)
            ? brushStepCount(brush_pos_prev, brush_pos, mode) : 1;

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
                obstacle_added = addObstacles(problem.obstacles, brush_pos_prev, brush_pos, n);
                break;
            }
            case ProblemEditMode::DEL_OBSTACLE: {
                deleteObstacles(problem.obstacles, brush_pos_prev, brush_pos, n);
                break;
            }
            default: {
                throw std::logic_error("Unhandled ProblemEditMode");
            }
        }
    }

    if (reset_obstacles) {
        problem.obstacles = {};
    }

    return {start_changed, obstacle_added};
}
