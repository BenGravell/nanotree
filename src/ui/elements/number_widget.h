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
    Vector2 pos;
    DeltaButtonGroup delta_button_group;

    NumberWidget(const int current, const std::vector<int>& options, const Vector2 pos)
        : current(current), options(options), pos(pos), delta_button_group(pos) {
    }

    void update() {
        const int idx_delta = delta_button_group.get();
        const int idx_old = int(std::lower_bound(options.begin(), options.end(), current) - options.begin());
        const int idx_new = std::clamp(idx_old + idx_delta, 0, int(options.size() - 1));
        current = options[idx_new];
    }
};
