#pragma once

#include <raylib.h>

#include <optional>

enum class SelectorMode {
    PLACE_START = 0,
    PLACE_GOAL = 1,
    DEL_OBSTACLE = 2,
    ADD_OBSTACLE = 3
};

struct Selector {
    SelectorMode mode;
    Rectangle place_start_button;
    Rectangle place_goal_button;
    Rectangle del_obstacle_button;
    Rectangle add_obstacle_button;

    std::optional<SelectorMode> getSelectorMode() const {
        if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return std::nullopt;
        }

        const Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, place_start_button)) {
            return SelectorMode::PLACE_START;
        }
        if (CheckCollisionPointRec(mouse, place_goal_button)) {
            return SelectorMode::PLACE_GOAL;
        }
        if (CheckCollisionPointRec(mouse, del_obstacle_button)) {
            return SelectorMode::DEL_OBSTACLE;
        }
        if (CheckCollisionPointRec(mouse, add_obstacle_button)) {
            return SelectorMode::ADD_OBSTACLE;
        }

        return std::nullopt;
    }

    void update() {
        mode = getSelectorMode().value_or(mode);
    }
};