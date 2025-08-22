#pragma once

#include <raylib.h>

#include <optional>
#include <map>

enum class SelectorMode {
    PLACE_START = 0,
    PLACE_GOAL = 1,
    DEL_OBSTACLE = 2,
    ADD_OBSTACLE = 3
};

struct Selector {
    SelectorMode mode;
    std::map<SelectorMode, Rectangle> rectangles;

    std::optional<SelectorMode> getSelectorMode() const {
        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return std::nullopt;
        }

        const Vector2 mouse = GetMousePosition();
        for (const auto& [rec_mode, rec] : rectangles) {
            if (CheckCollisionPointRec(mouse, rec)) {
                return rec_mode;
            }
        }

        return std::nullopt;
    }

    void update() {
        mode = getSelectorMode().value_or(mode);
    }
};
