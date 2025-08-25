#pragma once

#include <raylib.h>

#include "config.h"
#include "core/obstacle.h"
#include "planner/cost.h"
#include "planner/tree.h"
#include "ui/colors.h"
#include "ui/text.h"
#include "ui/timing.h"

static constexpr int STAT_BAR_X_MIN = 0;
static constexpr int STAT_BAR_X_MAX = STAT_BAR_X_MIN + STAT_BAR_WIDTH;
static constexpr int STAT_BAR_Y_MIN = 0;
static constexpr int STAT_BAR_Y_MAX = STAT_BAR_Y_MIN + STAT_BAR_HEIGHT;
static constexpr Rectangle STAT_BAR_REC = {STAT_BAR_X_MIN, STAT_BAR_Y_MIN, STAT_BAR_WIDTH, STAT_BAR_HEIGHT};

void DrawStatBar(const Tree& tree, const Path path, const Vector2 goal, const bool goal_reached, const Obstacles& obstacles, const DurationParts duration_parts, const int fps, const Font font) {
    // Background
    DrawRectangleRec(STAT_BAR_REC, COLOR_STAT_BAR_BACKGROUND);

    static constexpr int X = STAT_BAR_X_MIN + TEXT_MARGIN_WIDTH;

    static constexpr int ROW_0_Y = 0 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_1_Y = 1 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_2_Y = 2 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_3_Y = 3 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_4_Y = 4 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_5_Y = 5 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_6_Y = 6 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_7_Y = 7 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_8_Y = 8 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_9_Y = 9 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_10_Y = 10 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_11_Y = 11 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_12_Y = 12 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_13_Y = 13 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_14_Y = 14 * STAT_BAR_ROW_HEIGHT + (STAT_BAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;

    // TODO center all text

    DrawTextEx(font, std::string(goal_reached ? "Goal reached" : "Goal missed").c_str(), {X, ROW_1_Y}, TEXT_HEIGHT_STAT, 1, computeGoalColor(goal_reached));

    DrawTextEx(font, (std::to_string(std::lround(computeHeuristicCost(path.back(), goal))) + " path cost").c_str(), {X, ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);

    DrawTextEx(font, (std::to_string(tree.nodes.size()) + " nodes").c_str(), {X, ROW_4_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);

    DrawTextEx(font, (std::to_string(obstacles.size()) + " obstacles").c_str(), {X, ROW_5_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);

    DrawTextEx(font, TextFormat("%4d ms [grow]", int(1000.0f * duration_parts.grow)), {X, ROW_7_Y}, TEXT_HEIGHT_STAT, 1, COLOR_TIMING_STAT);
    DrawTextEx(font, TextFormat("%4d ms [carryover]", int(1000.0f * duration_parts.carryover)), {X, ROW_8_Y}, TEXT_HEIGHT_STAT, 1, COLOR_TIMING_STAT);
    DrawTextEx(font, TextFormat("%4d ms [draw]", int(1000.0f * duration_parts.draw)), {X, ROW_9_Y}, TEXT_HEIGHT_STAT, 1, COLOR_TIMING_STAT);
    DrawTextEx(font, TextFormat("%4d ms [total]", int(1000.0f * duration_parts.total)), {X, ROW_10_Y}, TEXT_HEIGHT_STAT, 1, COLOR_GRAY_192);

    DrawTextEx(font, TextFormat("%2i FPS", fps), {X, ROW_13_Y}, TEXT_HEIGHT_STAT, 1, computeFpsColor(fps));

    // Border
    DrawRectangleLinesEx(STAT_BAR_REC, 3, COLOR_STAT_BAR_BORDER);
}