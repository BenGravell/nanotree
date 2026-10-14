#pragma once

#include <stdexcept>

#include "core/ctrl_state.h"
#include "core/geometry.h"
#include "core/mouse_state.h"
#include "core/problem/problem.h"
#include "core/problem/problem_edit_mode.h"
#include "core/problem/problem_edits.h"

int brushStepCount(const Vector2 previous, const Vector2 current, const ProblemEditMode mode) {
    float spacing = 1.0f;
    if (mode == ProblemEditMode::ADD_OBSTACLE) spacing = 1.2 * OBSTACLE_SPACING_MIN;
    if (mode == ProblemEditMode::DEL_OBSTACLE) spacing = 0.5 * OBSTACLE_SPACING_MIN;
    return std::max(static_cast<int>(std::ceil(Vector2Distance(current, previous) / spacing)), 1);
}

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

void deleteObstacles(Obstacles& obstacles, const Vector2 previous, const Vector2 current, const int steps) {
    for (int i = 1; i <= steps; ++i) {
        const Vector2 pos = Vector2Lerp(previous, current, static_cast<float>(i) / steps);
        obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Vector2 o) {
            return Vector2Distance(o, pos) < OBSTACLE_DELETE_EFFECTIVE_RADIUS;
        }), obstacles.end());
    }
}

struct ProblemEditor {
    Vector2 previous_mouse_pos = clampToEnvironment({0, 0});
    ProblemEditMode previous_mode = ProblemEditMode::PLACE_GOAL;
    bool previous_active = false;

    ProblemEdits update(Problem& problem, const CtrlState& state, const MouseState& mouse) {
        const ProblemEditMode mode = state.problem_edit_mode;
        ProblemEdits edits{};
        const bool active = mouse.inside && mouse.down;
        if (active) {
            const int n = (mode == previous_mode && previous_active)
                ? brushStepCount(previous_mouse_pos, mouse.pos, mode) : 1;

            edits = applyEdit(problem, mode, mouse.pos, n);
        }

        if (state.reset_obstacles) {
            problem.obstacles = {};
        }

        previous_mouse_pos = mouse.pos;
        previous_mode = mode;
        previous_active = active;
        return edits;
    }

private:
    ProblemEdits applyEdit(Problem& problem, const ProblemEditMode mode, const Vector2 pos, const int steps) const {
        ProblemEdits edits{};
        switch (mode) {
            case ProblemEditMode::PLACE_START: {
                edits.start_changed = isStartChanged(problem.start, pos);
                if (edits.start_changed) {
                    problem.start = pos;
                }
                break;
            }
            case ProblemEditMode::PLACE_GOAL: {
                problem.goal = pos;
                break;
            }
            case ProblemEditMode::ADD_OBSTACLE: {
                edits.obstacle_added = addObstacles(problem.obstacles, previous_mouse_pos, pos, steps);
                break;
            }
            case ProblemEditMode::DEL_OBSTACLE: {
                deleteObstacles(problem.obstacles, previous_mouse_pos, pos, steps);
                break;
            }
            default: {
                throw std::logic_error("Unhandled ProblemEditMode");
            }
        }
        return edits;
    }
};
