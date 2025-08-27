#pragma once

#include <raylib.h>

#include "config.h"
#include "core/obstacle.h"
#include "planner/cost.h"
#include "planner/tree.h"
#include "ui/colors.h"
#include "ui/text.h"
#include "ui/timing.h"

static constexpr int STAT_BAR_BUTTON_X_MIN = STAT_BAR_X_MIN + BUTTON_SPACING_X;
static constexpr int STAT_BAR_BUTTON_WIDTH = STAT_BAR_WIDTH - 2 * BUTTON_SPACING_X;

// TODO move
void GuiLabelColor(Rectangle bounds, const char* text, const Color color) {
    Color color_original = GetColor(GuiGetStyle(LABEL, TEXT_COLOR_NORMAL));
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(color));
    GuiLabel(bounds, text);
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(color_original));
}

void DrawStatBar(const Tree& tree, const Path path, const Vector2 goal, const bool goal_reached, const Obstacles& obstacles, const DurationParts duration_parts, const int fps) {
    // Background
    DrawRectangleRec(STAT_BAR_REC, COLOR_STAT_BAR_BACKGROUND);

    static constexpr int ROW_0_Y = STAT_BAR_Y_MIN + 0 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_1_Y = STAT_BAR_Y_MIN + 1 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_2_Y = STAT_BAR_Y_MIN + 2 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_3_Y = STAT_BAR_Y_MIN + 3 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_4_Y = STAT_BAR_Y_MIN + 4 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_5_Y = STAT_BAR_Y_MIN + 5 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_6_Y = STAT_BAR_Y_MIN + 6 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_7_Y = STAT_BAR_Y_MIN + 7 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_8_Y = STAT_BAR_Y_MIN + 8 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_9_Y = STAT_BAR_Y_MIN + 9 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_10_Y = STAT_BAR_Y_MIN + 10 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_11_Y = STAT_BAR_Y_MIN + 11 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_12_Y = STAT_BAR_Y_MIN + 12 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_13_Y = STAT_BAR_Y_MIN + 13 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_14_Y = STAT_BAR_Y_MIN + 14 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_15_Y = STAT_BAR_Y_MIN + 15 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;
    static constexpr int ROW_16_Y = STAT_BAR_Y_MIN + 16 * STAT_BAR_ROW_HEIGHT + STAT_BAR_ROW_HEIGHT;

    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_1_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, goal_reached ? "Goal reached" : "Goal missed", computeGoalColor(goal_reached));

    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_2_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, (std::to_string(std::lround(computeHeuristicCost(path.back(), goal))) + " path cost").c_str(), COLOR_STAT);

    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_4_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, (std::to_string(tree.nodes.size()) + " nodes").c_str(), COLOR_STAT);

    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_5_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, (std::to_string(obstacles.size()) + " obstacles").c_str(), COLOR_STAT);

    static constexpr int TIMING_TEXT_HEIGHT = 30;
    GuiSetStyle(DEFAULT, TEXT_SIZE, TIMING_TEXT_HEIGHT);

    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_7_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT / 2}, TextFormat("%4d ms [grow]", int(1000.0f * duration_parts.grow)), COLOR_TIMING_STAT);
    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_7_Y + STAT_BAR_ROW_HEIGHT / 2, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT / 2}, TextFormat("%4d ms [carryover]", int(1000.0f * duration_parts.carryover)), COLOR_TIMING_STAT);
    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_8_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT / 2}, TextFormat("%4d ms [draw]", int(1000.0f * duration_parts.draw)), COLOR_TIMING_STAT);
    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_8_Y + STAT_BAR_ROW_HEIGHT / 2, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT / 2}, TextFormat("%4d ms [total]", int(1000.0f * duration_parts.total)), COLOR_TIMING_STAT);

    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);

    GuiLabelColor((Rectangle){STAT_BAR_BUTTON_X_MIN, ROW_10_Y, STAT_BAR_BUTTON_WIDTH, STAT_BAR_ROW_HEIGHT}, TextFormat("%2i FPS", fps), computeFpsColor(fps));

    // Border
    DrawRectangleLinesEx(STAT_BAR_REC, BORDER_THICKNESS, COLOR_STAT_BAR_BORDER);
}