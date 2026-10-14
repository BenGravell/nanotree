#pragma once

#include "core/problem/problem_edits.h"
#include "core/tree_edits.h"

struct ActionSettings {
  ProblemEdits problem_edits;
  TreeEdits tree_edits;
};

static constexpr ActionSettings PREP_ACTION_SETTINGS = {
    .problem_edits = {.start_changed = false, .obstacle_added = false},
    .tree_edits = {.should_reset = false, .should_grow = true},
};
