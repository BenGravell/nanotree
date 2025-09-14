#pragma once

#include <raylib.h>

#include "config.h"
#include "core/obstacle.h"
#include "core/timing_parts.h"
#include "planner/cost.h"
#include "planner/tree.h"
#include "ui/colors.h"
#include "ui/gui_label.h"

static constexpr int STAT_BAR_BUTTON_X_MIN = STAT_BAR_X_MIN + BUTTON_SPACING_X;
static constexpr int STAT_BAR_BUTTON_WIDTH = STAT_BAR_WIDTH - 2 * BUTTON_SPACING_X;
static constexpr int STAT_BAR_HALF_ROW_HEIGHT = STAT_BAR_ROW_HEIGHT / 2;

void DrawStatBar(const Problem& problem, const Planner& planner, const Vector2 brush_pos, const CtrlState& ctrl_state, const bool goal_reached, const DurationParts duration_parts) {
    // Background
    DrawRectangleRec(STAT_BAR_REC, COLOR_STAT_BAR_BACKGROUND);

    static constexpr int ROW_0_Y = STAT_BAR_Y_MIN + 0 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_1_Y = STAT_BAR_Y_MIN + 1 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_2_Y = STAT_BAR_Y_MIN + 2 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_3_Y = STAT_BAR_Y_MIN + 3 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_4_Y = STAT_BAR_Y_MIN + 4 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_5_Y = STAT_BAR_Y_MIN + 5 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_6_Y = STAT_BAR_Y_MIN + 6 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_7_Y = STAT_BAR_Y_MIN + 7 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_8_Y = STAT_BAR_Y_MIN + 8 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_9_Y = STAT_BAR_Y_MIN + 9 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_10_Y = STAT_BAR_Y_MIN + 10 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_11_Y = STAT_BAR_Y_MIN + 11 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_12_Y = STAT_BAR_Y_MIN + 12 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_13_Y = STAT_BAR_Y_MIN + 13 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_14_Y = STAT_BAR_Y_MIN + 14 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_15_Y = STAT_BAR_Y_MIN + 15 * STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_16_Y = STAT_BAR_Y_MIN + 16 * STAT_BAR_ROW_HEIGHT;

    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_1_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Goal", goal_reached ? "Reached" : "Missed", computeGoalColor(goal_reached));

    const float path_cost_to_come = planner.path.back()->cost_to_come;
    const float path_cost_to_go = computeCost(planner.path.back()->pos, problem.goal);
    const float path_cost = path_cost_to_come + path_cost_to_go;
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_3_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Cost", TextFormat("%d", std::lround(path_cost)), goal_reached ? COLOR_STAT : COLOR_PATH_GOAL_NOT_REACHED);

    GuiSetStyle(DEFAULT, TEXT_SIZE, SMALL_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_4_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Cost-to-Come", TextFormat("%d", std::lround(path_cost_to_come)), COLOR_MINOR_STAT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_4_Y + STAT_BAR_HALF_ROW_HEIGHT, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Cost-to-Go", TextFormat("%d", std::lround(path_cost_to_go)), COLOR_MINOR_STAT);

    // Node counts
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_6_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Nodes", TextFormat("%d", planner.tree.nodes.size()), COLOR_STAT);

    GuiSetStyle(DEFAULT, TEXT_SIZE, SMALL_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_7_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Path", TextFormat("%d", planner.path.size()), COLOR_MINOR_STAT);

    // Count low and high cost nodes.
    // TODO factor this out to a tree stats struct and compute just once, pass to tree draw func.
    int num_nodes_lo_cost = 0;
    int num_nodes_hi_cost = 0;
    for (const NodePtr& node : planner.tree.nodes) {
        const float cost = node->estimateCostTo(problem.goal);
        if (cost < path_cost) {
            num_nodes_lo_cost++;
        } else {
            num_nodes_hi_cost++;
        }
    }

    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_7_Y + STAT_BAR_HALF_ROW_HEIGHT, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Low Cost", TextFormat("%d", num_nodes_lo_cost), COLOR_MINOR_STAT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_8_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "High Cost", TextFormat("%d", num_nodes_hi_cost), COLOR_MINOR_STAT);

    // Env info
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelValueColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_10_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Obstacles", TextFormat("%d", problem.obstacles.size()), COLOR_STAT);

    // Timing parts
    // TODO factor this block to a function
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_12_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, "Frame", duration_parts.total, true);

    GuiSetStyle(DEFAULT, TEXT_SIZE, SMALL_TEXT_HEIGHT);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_13_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Grow", duration_parts.grow, false);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_13_Y + STAT_BAR_HALF_ROW_HEIGHT, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Carry", duration_parts.carry, false);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_14_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Cull", duration_parts.cull, false);
    GuiLabelTimingStat((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_14_Y + STAT_BAR_HALF_ROW_HEIGHT, STAT_BAR_BUTTON_WIDTH, STAT_BAR_HALF_ROW_HEIGHT}, "Draw", duration_parts.draw, false);

    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);

    // Border
    DrawRectangleLinesEx(STAT_BAR_REC, BORDER_THICKNESS, COLOR_STAT_BAR_BORDER);
}