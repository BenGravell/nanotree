#pragma once

#include <raylib.h>

#include "ui/problem_edit_mode.h"

enum class TreeGrowthMode {
    ALWAYS = 0,
    UNTIL_GOAL_REACHED = 1,
    NEVER = 2
};

struct Visibility {
    bool tree = true;
    bool path = true;
    bool obstacles = true;
};

struct CtrlState {
    ProblemEditMode problem_edit_mode = ProblemEditMode::PLACE_GOAL;
    bool snap_to_grid = false;
    bool reset_obstacles = false;
    TreeGrowthMode tree_growth_mode = TreeGrowthMode::UNTIL_GOAL_REACHED;
    bool tree_should_reset = false;
    bool tree_should_grow = false;
    bool rewire_enabled = true;
    int num_samples_ix = 5;
    int num_carry_ix = 7;    
    Visibility visibility;
};