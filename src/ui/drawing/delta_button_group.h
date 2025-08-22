#pragma once

#include <raylib.h>

#include <map>

#include "config.h"
#include "ui/colors.h"
#include "ui/drawing/shapes.h"
#include "ui/elements/delta_button_group.h"

void DrawDeltaButtonGroup(const DeltaButtonGroup& b) {
    for (const auto& [delta, rec] : b.rectangles) {
        DrawRectangleRec(rec, COLOR_BUTTON_BACKGROUND_INACTIVE);
        DrawRectangleLinesEx(rec, 3.0f, COLOR_BUTTON_BORDER_INACTIVE);
        const int delta_int = static_cast<int>(delta);
        const int delta_sign = delta_int > 0 ? 1 : -1;
        const float dir = 0.6f * delta_sign;
        if (std::abs(delta_int) > 1) {
            DrawDoubleChevron(rec, dir, COLOR_BUTTON_CONTENT_INACTIVE);
        } else {
            DrawChevron(rec, dir, COLOR_BUTTON_CONTENT_INACTIVE);
        }
    }
}