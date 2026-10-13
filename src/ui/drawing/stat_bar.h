#pragma once

#include <raylib.h>

#include "config.h"
#include "core/problem/obstacle.h"
#include "core/timing/timing_parts.h"
#include "planner/cost.h"
#include "planner/planner.h"
#include "ui/colors.h"
#include "ui/gui_label.h"

static constexpr int STAT_BAR_BUTTON_X_MIN = STAT_BAR_X_MIN + BUTTON_SPACING_X;
static constexpr int STAT_BAR_BUTTON_WIDTH = STAT_BAR_WIDTH - 2 * BUTTON_SPACING_X;
static constexpr int STAT_BAR_HALF_ROW_HEIGHT = STAT_BAR_ROW_HEIGHT / 2;

static constexpr int STAT_BAR_ROW_1_Y = STAT_BAR_Y_MIN + 1 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_3_Y = STAT_BAR_Y_MIN + 3 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_4_Y = STAT_BAR_Y_MIN + 4 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_6_Y = STAT_BAR_Y_MIN + 6 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_7_Y = STAT_BAR_Y_MIN + 7 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_8_Y = STAT_BAR_Y_MIN + 8 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_10_Y = STAT_BAR_Y_MIN + 10 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_12_Y = STAT_BAR_Y_MIN + 12 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_13_Y = STAT_BAR_Y_MIN + 13 * STAT_BAR_ROW_HEIGHT;
static constexpr int STAT_BAR_ROW_14_Y = STAT_BAR_Y_MIN + 14 * STAT_BAR_ROW_HEIGHT;

void DrawPathStats(const float path_cost_to_come, const float path_cost_to_go, const bool goal_reached) {
    const float path_cost = path_cost_to_come + path_cost_to_go;
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_1_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Goal", goal_reached ? "Reached" : "Missed", computeGoalColor(goal_reached));

    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_3_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Cost", TextFormat("%d", std::lround(path_cost)), goal_reached ? COLOR_STAT : COLOR_PATH_GOAL_NOT_REACHED);

    GuiSetStyle(DEFAULT, TEXT_SIZE, SMALL_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_4_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Cost-to-Come", TextFormat("%d", std::lround(path_cost_to_come)), COLOR_MINOR_STAT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_4_Y + STAT_BAR_HALF_ROW_HEIGHT, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Cost-to-Go", TextFormat("%d", std::lround(path_cost_to_go)), COLOR_MINOR_STAT);
}

void DrawNodeStats(const Planner& planner, const Vector2 goal, const float path_cost) {
    // Node counts
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_6_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Nodes", TextFormat("%d", planner.tree.nodes.size()), COLOR_STAT);

    GuiSetStyle(DEFAULT, TEXT_SIZE, SMALL_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_7_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Path", TextFormat("%d", planner.path.size()), COLOR_MINOR_STAT);

    // Count low and high cost nodes.
    // TODO factor this out to a tree stats struct and compute just once, pass to tree draw func.
    int num_nodes_lo_cost = 0;
    int num_nodes_hi_cost = 0;
    for (const NodePtr& node : planner.tree.nodes) {
        const float cost = node->estimateCostTo(goal);
        if (cost < path_cost) {
            num_nodes_lo_cost++;
        } else {
            num_nodes_hi_cost++;
        }
    }

    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_7_Y + STAT_BAR_HALF_ROW_HEIGHT, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Low Cost", TextFormat("%d", num_nodes_lo_cost), COLOR_MINOR_STAT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_8_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "High Cost", TextFormat("%d", num_nodes_hi_cost), COLOR_MINOR_STAT);
}

void DrawTimingStats(const DurationParts duration_parts) {
    // Timing parts
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_12_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Frame", duration_parts.total, true);

    GuiSetStyle(DEFAULT, TEXT_SIZE, SMALL_TEXT_HEIGHT);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_13_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Grow", duration_parts.grow, false);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_13_Y + STAT_BAR_HALF_ROW_HEIGHT, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Carry", duration_parts.carry, false);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_14_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Cull", duration_parts.cull, false);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_14_Y + STAT_BAR_HALF_ROW_HEIGHT, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Draw", duration_parts.draw, false);
}

void DrawStatBar(const Problem& problem, const Planner& planner, const bool goal_reached, const DurationParts duration_parts) {
    DrawRectangleRec(STAT_BAR_REC, COLOR_STAT_BAR_BACKGROUND);
    const float path_cost_to_come = planner.path.back()->cost_to_come;
    const float path_cost_to_go = computeCost(planner.path.back()->pos, problem.goal);
    const float path_cost = path_cost_to_come + path_cost_to_go;
    DrawPathStats(path_cost_to_come, path_cost_to_go, goal_reached);
    DrawNodeStats(planner, problem.goal, path_cost);

    // Env info
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, STAT_BAR_ROW_10_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Obstacles", TextFormat("%d", problem.obstacles.size()), COLOR_STAT);

    DrawTimingStats(duration_parts);
    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);

    // Border
    DrawRectangleLinesEx(STAT_BAR_REC, BORDER_THICKNESS, COLOR_STAT_BAR_BORDER);
}