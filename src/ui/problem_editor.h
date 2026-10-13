#pragma once

#include "core/ctrl_state.h"
#include "core/problem/problem_editor.h"
#include "planner/plan_settings.h"

PlanSettings getPlanSettings(const CtrlState& state) {
    return {NUM_CARRY_OPTIONS[state.num_carry_ix], NUM_SAMPLES_OPTIONS[state.num_samples_ix], state.rewire_enabled};
}

struct ProblemEditor {
    Vector2 previous = clampToEnvironment({0, 0});
    ProblemEditMode previous_mode = ProblemEditMode::PLACE_GOAL;
    bool active = false;

    Vector2 brushPosition(const bool snap) const {
        Vector2 pos = clampToEnvironment(GetMousePosition());
        if (snap) {
            pos = snapToGridCenter(pos, CELL_SIZE);
        }
        return pos;
    }

    ProblemEdits update(Problem& problem, const CtrlState& state, const Vector2 pos) {
        const bool down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        const bool inside = insideEnvironment(GetMousePosition());
        const auto edits = editProblem(problem, pos, previous, down, state.problem_edit_mode,
                                       previous_mode, inside, state.reset_obstacles, active);
        previous = pos;
        previous_mode = state.problem_edit_mode;
        active = inside && down;
        return edits;
    }
};
