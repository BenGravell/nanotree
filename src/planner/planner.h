#pragma once

#include "core/problem.h"
#include "core/problem_edits.h"
#include "core/timing_parts.h"
#include "core/tree_edits.h"
#include "planner/node.h"
#include "planner/path.h"
#include "planner/tree.h"

struct PlanSettings {
    int num_carry;
    int num_samples;
    bool rewire_enabled;
};

struct ActionSettings {
    ProblemEdits problem_edits;
    TreeEdits tree_edits;
};

struct Planner {
    Tree tree;
    Path path;
    PlanningTimingParts timing;

    void plan(const Problem& problem, const PlanSettings& plan_settings, const ActionSettings& action_settings) {
        if (action_settings.tree_edits.should_reset) {
            tree.reset(problem.start);
        }

        if (action_settings.problem_edits.start_changed) {
            tree.resetRoot(problem, path);
        }

        timing.carry.start();
        const bool do_carry = action_settings.tree_edits.should_grow && !action_settings.tree_edits.should_reset;
        if (do_carry) {
            tree.carry(path, plan_settings.num_carry, problem.obstacles);
        }
        timing.carry.record();

        timing.cull.start();
        const bool do_cull = action_settings.problem_edits.obstacle_added || action_settings.problem_edits.start_changed;
        if (do_cull) {
            tree.cullByObstacles(problem.obstacles);
        }
        timing.cull.record();

        timing.grow.start();
        if (action_settings.tree_edits.should_grow) {
            tree.grow(problem, plan_settings.num_samples, plan_settings.rewire_enabled);
        }
        timing.grow.record();

        path = extractPath(tree.nodes, problem);
    }

    void prep(const Problem& problem, const PlanSettings& plan_settings) {
        tree.reset(problem.start);

        static constexpr bool start_changed = false;
        static constexpr bool obstacle_added = false;
        static constexpr ProblemEdits problem_edits = {start_changed, obstacle_added};

        static constexpr bool tree_should_reset = false;
        static constexpr bool tree_should_grow = true;
        static constexpr TreeEdits tree_edits = {tree_should_reset, tree_should_grow};

        static constexpr ActionSettings action_settings = {problem_edits, tree_edits};

        for (int i = 0; i < NUM_PREP_ITERATIONS; ++i) {
            plan(problem, plan_settings, action_settings);
        }    
    }
};
