#pragma once

#include <raylib.h>

struct ToggleButton {
    bool engaged;
    Rectangle rec;

    void update() {
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return;
        }
        if (CheckCollisionPointRec(GetMousePosition(), rec)) {
            engaged = !engaged;
        }
    }
};
