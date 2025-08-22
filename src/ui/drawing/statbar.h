#pragma once

#include <raylib.h>

#include "config.h"
#include "core/obstacle.h"
#include "planner/cost.h"
#include "planner/tree.h"
#include "ui/colors.h"
#include "ui/text.h"
#include "ui/timing.h"

static constexpr Rectangle STATBAR_REC = {ENVIRONMENT_WIDTH, 0, STATBAR_WIDTH, STATBAR_HEIGHT};

void DrawStatBar(const Tree& tree, const Path path, const Vector2 goal, const bool goal_reached, const Obstacles& obstacles, const DurationParts duration_parts, const int fps, const Font font) {
    // Background
    DrawRectangleRec(STATBAR_REC, COLOR_STATBAR_BACKGROUND);

    static constexpr int X = ENVIRONMENT_WIDTH + TEXT_MARGIN_WIDTH;

    static constexpr int ROW_0_Y = 0 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_1_Y = 1 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_2_Y = 2 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_3_Y = 3 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_4_Y = 4 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_5_Y = 5 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_6_Y = 6 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_7_Y = 7 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_8_Y = 8 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_9_Y = 9 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_10_Y = 10 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_11_Y = 11 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_12_Y = 12 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;
    static constexpr int ROW_13_Y = 13 * STATBAR_ROW_HEIGHT + (STATBAR_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;

    // TODO center all text

    DrawTextEx(font, std::string(goal_reached ? "Goal reached" : "Goal missed").c_str(), {X, ROW_1_Y}, TEXT_HEIGHT_STAT, 1, computeGoalColor(goal_reached));

    DrawTextEx(font, (std::to_string(std::lround(computeHeuristicCost(path.back(), goal))) + " path cost").c_str(), {X, ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);

    DrawTextEx(font, (std::to_string(tree.nodes.size()) + " nodes").c_str(), {X, ROW_4_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);

    DrawTextEx(font, (std::to_string(obstacles.size()) + " obstacles").c_str(), {X, ROW_5_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);

    DrawTextEx(font, TextFormat("%4d ms [grow]", int(1000.0f * duration_parts.grow_tree)), {X, ROW_7_Y}, TEXT_HEIGHT_STAT, 1, COLOR_TIMING_STAT);
    DrawTextEx(font, TextFormat("%4d ms [carryover]", int(1000.0f * duration_parts.carryover_tree)), {X, ROW_8_Y}, TEXT_HEIGHT_STAT, 1, COLOR_TIMING_STAT);
    DrawTextEx(font, TextFormat("%4d ms [draw]", int(1000.0f * duration_parts.draw)), {X, ROW_9_Y}, TEXT_HEIGHT_STAT, 1, COLOR_TIMING_STAT);
    DrawTextEx(font, TextFormat("%4d ms [total]", int(1000.0f * duration_parts.total)), {X, ROW_10_Y}, TEXT_HEIGHT_STAT, 1, COLOR_GRAY_192);

    DrawTextEx(font, TextFormat("%2i FPS", fps), {X, ROW_13_Y}, TEXT_HEIGHT_STAT, 1, computeFpsColor(fps));

    // Border
    DrawRectangleLinesEx(STATBAR_REC, 3, COLOR_STATBAR_BORDER);
}