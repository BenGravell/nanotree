#pragma once

#include <raylib.h>

#include "ui/selector_mode.h"

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
    SelectorMode selector_mode = SelectorMode::PLACE_GOAL;
    TreeGrowthMode tree_growth_mode = TreeGrowthMode::UNTIL_GOAL_REACHED;
    bool tree_should_reset = false;
    bool tree_should_grow = false;
    int num_samples_ix = 9;
    int num_carryover_ix = 11;
    bool snap_to_grid = false;
    Visibility visibility;
};