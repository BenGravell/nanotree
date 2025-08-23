#pragma once

#include <raylib.h>

#include <map>

#include "config.h"
#include "ui/colors.h"
#include "ui/drawing/momentary_button.h"
#include "ui/drawing/shapes.h"
#include "ui/elements/delta_button_group.h"

void DrawDeltaButtonGroup(const DeltaButtonGroup& b) {
    for (const auto& [delta_size, button] : b.button_map) {
        DrawMomentaryButton(button);
        const int delta_int = static_cast<int>(delta_size);
        const int delta_sign = delta_int > 0 ? 1 : -1;
        const float dir = 0.6f * delta_sign;
        if (std::abs(delta_int) > 1) {
            DrawDoubleChevron(button.rec, dir, COLOR_BUTTON_CONTENT_INACTIVE);
        } else {
            DrawChevron(button.rec, dir, COLOR_BUTTON_CONTENT_INACTIVE);
        }
    }
}