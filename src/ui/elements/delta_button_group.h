#pragma once

#include <raylib.h>

#include <map>

#include "config.h"

enum class DeltaSize {
    DEC_LARGE = -3,
    DEC_SMALL = -1,
    INC_SMALL = 1,
    INC_LARGE = 3
};

struct DeltaButtonGroup {
    std::map<DeltaSize, Rectangle> rectangles;

    DeltaButtonGroup(const Vector2 pos) {
        rectangles[DeltaSize::DEC_LARGE] = {pos.x + 0 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN), pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT};
        rectangles[DeltaSize::DEC_SMALL] = {pos.x + 1 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN), pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT};
        rectangles[DeltaSize::INC_SMALL] = {pos.x + 2 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN), pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT};
        rectangles[DeltaSize::INC_LARGE] = {pos.x + 3 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN), pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT};
    }

    int get() {
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            return 0;
        }

        const Vector2 mouse = GetMousePosition();
        for (const auto& [delta, rec] : rectangles) {
            if (CheckCollisionPointRec(mouse, rec)) {
                return static_cast<int>(delta);
            }
        }
        return 0;
    }
};