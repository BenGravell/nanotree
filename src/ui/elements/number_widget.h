#pragma once

#include <raylib.h>

#include <algorithm>
#include <vector>

#include "config.h"
#include "ui/colors.h"
#include "ui/elements/delta_button_group.h"
#include "ui/elements/toggle_button.h"
#include "ui/input.h"

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
