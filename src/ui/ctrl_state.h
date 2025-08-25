#pragma once

#include <raylib.h>

#include "ui/selector_mode.h"

enum class TreeGrowthMode {
    ALWAYS = 0,
    ON_TRIGGER = 1,
    NEVER = 2
};

struct CtrlState {
    SelectorMode selector_mode = SelectorMode::PLACE_GOAL;
    TreeGrowthMode tree_growth_mode = TreeGrowthMode::ON_TRIGGER;
    bool tree_should_reset = false;
    bool tree_should_grow = false;
    bool carryover_path = true;
    int num_samples = 200;
    int num_carryover = 2000;
    bool num_samples_edit_mode = false;
    bool num_carryover_edit_mode = false;
};