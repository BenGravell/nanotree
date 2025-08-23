#pragma once

#include <raylib.h>

#include <map>

#include "config.h"
#include "ui/elements/momentary_button.h"

enum class DeltaSize {
    DEC_LARGE = -3,
    DEC_SMALL = -1,
    INC_SMALL = 1,
    INC_LARGE = 3
};

struct DeltaButtonGroup {
    std::map<DeltaSize, MomentaryButton> button_map;

    DeltaButtonGroup(const Vector2 pos) {
        button_map[DeltaSize::DEC_LARGE] = MomentaryButton{{pos.x + 0 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN), pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT}, MOMENT_DURATION};
        button_map[DeltaSize::DEC_SMALL] = MomentaryButton{{pos.x + 1 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN), pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT}, MOMENT_DURATION};
        button_map[DeltaSize::INC_SMALL] = MomentaryButton{{pos.x + 2 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN), pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT}, MOMENT_DURATION};
        button_map[DeltaSize::INC_LARGE] = MomentaryButton{{pos.x + 3 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN), pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT}, MOMENT_DURATION};
    }

    int get() {
        const bool lmb_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        int delta = 0;
        const Vector2 mouse = GetMousePosition();
        for (auto& [delta_size, button] : button_map) {
            const bool button_pressed = lmb_pressed && button.CheckCollisionPoint(mouse);
            button.update(button_pressed);
            if (button_pressed) {
                delta = static_cast<int>(delta_size);
            }
        }
        return delta;
    }
};