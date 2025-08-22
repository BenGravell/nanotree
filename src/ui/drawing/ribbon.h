#pragma once

#include <raylib.h>

#include "config.h"
#include "planner/tree.h"
#include "ui/colors.h"

void DrawRibbon(const int num_samples, const int num_carryover, const SelectorMode mode, const std::vector<Rectangle>& button_rectangles, const Font font) {
    const Rectangle rec{0, ENVIRONMENT_HEIGHT, RIBBON_WIDTH, RIBBON_HEIGHT};

    // Background
    DrawRectangleRec(rec, COLOR_RIBBON_BACKGROUND);

    // Separators between columns
    for (int i = 1; i < 4; ++i) {
        DrawLineEx({float(i) * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 10}, {float(i) * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 2 * RIBBON_ROW_HEIGHT - 10}, 3, Color{128, 128, 128, 255});
    }

    // TODO use the Rectangle objects directly
    // TODO make class for clickable button to handle color of button and text

    // TODO access rectangle by enum mode explicitly instead of relying on integer cast and index alignment
    // highlight the selected mode cell
    for (int m = 0; m <= 3; ++m) {
        const Rectangle& rec = button_rectangles[m];
        const bool active = (m == int(mode));
        const Color fill = active ? COLOR_BUTTON_BACKGROUND_ACTIVE : COLOR_BUTTON_BACKGROUND_INACTIVE;
        const Color border = active ? COLOR_BUTTON_BORDER_ACTIVE : COLOR_BUTTON_BORDER_INACTIVE;
        DrawRectangleRec(rec, fill);
        DrawRectangleLinesEx(rec, 3, border);
    }

    static constexpr int TEXT_MARGIN_WIDTH = 20;

    static constexpr int RIBBON_ROW_1_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_CONTROL_MODE) / 2;
    static constexpr int RIBBON_ROW_2_Y = ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;

    // TODO center all text

    DrawTextEx(font, ("Samples = " + std::to_string(num_samples)).c_str(), {2.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);
    DrawTextEx(font, ("Carryover = " + std::to_string(num_carryover)).c_str(), {3.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);

    // row 1a
    DrawTextEx(font, "Place start", {0.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::PLACE_START) ? COLOR_BUTTON_BACKGROUND_INACTIVE : COLOR_BUTTON_BACKGROUND_ACTIVE);
    DrawTextEx(font, "Place goal", {0.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::PLACE_GOAL) ? COLOR_BUTTON_BACKGROUND_INACTIVE : COLOR_BUTTON_BACKGROUND_ACTIVE);
    DrawTextEx(font, "- Obstacle", {1.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::DEL_OBSTACLE) ? COLOR_BUTTON_BACKGROUND_INACTIVE : COLOR_BUTTON_BACKGROUND_ACTIVE);
    DrawTextEx(font, "+ Obstacle", {1.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::ADD_OBSTACLE) ? COLOR_BUTTON_BACKGROUND_INACTIVE : COLOR_BUTTON_BACKGROUND_ACTIVE);

    // Border
    DrawRectangleLinesEx(rec, 3, COLOR_RIBBON_BORDER);
}