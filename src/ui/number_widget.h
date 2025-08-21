#pragma once

#include <raylib.h>

#include <algorithm>
#include <map>
#include <vector>

#include "ui/colors.h"
#include "ui/input.h"

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

struct NumberWidget {
    int current;
    std::vector<int> options;
    bool scrollable;
    Vector2 pos;
    DeltaButtonGroup delta_button_group;
    ToggleButton scroll_toggle_button;

    NumberWidget(const int current, const std::vector<int>& options, const bool scrollable, const Vector2 pos)
        : current(current), options(options), scrollable(scrollable), pos(pos), delta_button_group(pos), scroll_toggle_button({false, {pos.x + 4 * (DELTA_BUTTON_WIDTH + BUTTON_MARGIN) + BUTTON_MARGIN, pos.y, DELTA_BUTTON_WIDTH, DELTA_BUTTON_HEIGHT}}) {
    }

    void update() {
        scroll_toggle_button.update();
        scrollable = scroll_toggle_button.engaged;
        const int idx_delta_mouse_wheel = scrollable ? GetMouseWheelMoveSign() : 0;
        const int idx_delta_button_group = delta_button_group.get();
        const int idx_delta = (idx_delta_mouse_wheel != 0) ? idx_delta_mouse_wheel : idx_delta_button_group;
        const int idx_old = int(std::lower_bound(options.begin(), options.end(), current) - options.begin());
        const int idx_new = std::clamp(idx_old + idx_delta, 0, int(options.size() - 1));
        current = options[idx_new];
    }
};
