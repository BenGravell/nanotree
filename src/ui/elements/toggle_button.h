#pragma once

#include <raylib.h>

struct ToggleButton {
    Rectangle rec;
    bool engaged = false;

    void update() {
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return;
        }
        if (CheckCollisionPointRec(GetMousePosition(), rec)) {
            engaged = !engaged;
        }
    }
};
