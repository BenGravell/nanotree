#pragma once

#include <raylib.h>

#include "core/problem_edit_mode.h"
#include "core/tree_edits.h"
#include "core/tree_growth_mode.h"
#include "core/visibility.h"

struct CtrlState {
    ProblemEditMode problem_edit_mode = ProblemEditMode::PLACE_GOAL;
    bool snap_to_grid = false;
    bool reset_obstacles = false;
    TreeGrowthMode tree_growth_mode = TreeGrowthMode::UNTIL_GOAL_REACHED;
    TreeEdits tree_edits = {false, false};
    bool rewire_enabled = true;
    int num_samples_ix = 5;
    int num_carry_ix = 7;
    Visibility visibility;
};
