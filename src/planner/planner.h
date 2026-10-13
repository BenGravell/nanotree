#pragma once

#include "core/problem/problem.h"
#include "core/timing/timing_parts.h"
#include "planner/action_settings.h"
#include "planner/plan_settings.h"
#include "planner/path.h"
#include "planner/tree.h"

struct Planner {
    Tree tree;
    Path path;
    PlanningTimingParts timing;

    void plan(const Problem& problem, const PlanSettings& plan_settings, const ActionSettings& action_settings) {
        reset(problem, action_settings);
        carry(problem, plan_settings, action_settings);
        cull(problem, action_settings);
        grow(problem, plan_settings, action_settings);
        path = extractPath(tree.nodes, problem);
    }

    void prep(const Problem& problem, const PlanSettings& plan_settings) {
        tree.reset(problem.start);

        for (int i = 0; i < NUM_PREP_ITERATIONS; ++i) {
            plan(problem, plan_settings, PREP_ACTION_SETTINGS);
        }
    }

private:
    void reset(const Problem& problem, const ActionSettings& action_settings) {
        if (action_settings.tree_edits.should_reset) {
            tree.reset(problem.start);
        }

        if (action_settings.problem_edits.start_changed) {
            tree.resetRoot(problem, path);
        }
    }

    void carry(const Problem& problem, const PlanSettings& plan_settings, const ActionSettings& action_settings) {
        timing.carry.start();
        const bool do_carry = action_settings.tree_edits.should_grow && !action_settings.tree_edits.should_reset;
        if (do_carry) {
            tree.carry(path, plan_settings.num_carry, problem.obstacles);
        }
        timing.carry.record();
    }

    void cull(const Problem& problem, const ActionSettings& action_settings) {
        timing.cull.start();
        const bool do_cull = action_settings.problem_edits.obstacle_added || action_settings.problem_edits.start_changed;
        if (do_cull) {
            tree.cullByObstacles(problem.obstacles);
        }
        timing.cull.record();
    }

    void grow(const Problem& problem, const PlanSettings& plan_settings, const ActionSettings& action_settings) {
        timing.grow.start();
        if (action_settings.tree_edits.should_grow) {
            tree.grow(problem, plan_settings.num_samples, plan_settings.rewire_enabled);
        }
        timing.grow.record();
    }
};
