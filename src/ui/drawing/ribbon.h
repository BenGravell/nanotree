#pragma once

#include <raylib.h>

#include "config.h"
#include "planner/tree.h"
#include "ui/colors.h"
#include "ui/drawing/selector.h"

static constexpr Rectangle RIBBON_REC = {0, ENVIRONMENT_HEIGHT, RIBBON_WIDTH, RIBBON_HEIGHT};

void DrawRibbon(const NumberWidget& num_samples_widget, const NumberWidget& num_carryover_widget, const Selector& selector, const Font font) {
    // Background
    DrawRectangleRec(RIBBON_REC, COLOR_RIBBON_BACKGROUND);

    // Separators between columns
    for (int i = 1; i < 4; ++i) {
        DrawLineEx({float(i) * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 10}, {float(i) * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 2 * RIBBON_ROW_HEIGHT - 10}, 3, Color{128, 128, 128, 255});
    }

    DrawSelector(selector, font);
    DrawNumberWidget(num_samples_widget);
    DrawNumberWidget(num_carryover_widget);

    // TODO center all text

    DrawTextEx(font, ("Samples = " + std::to_string(num_samples_widget.current)).c_str(), {2.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);
    DrawTextEx(font, ("Carryover = " + std::to_string(num_carryover_widget.current)).c_str(), {3.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_STAT);

    // Border
    DrawRectangleLinesEx(RIBBON_REC, 3, COLOR_RIBBON_BORDER);
}