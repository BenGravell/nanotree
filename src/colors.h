#pragma once

#include <raylib.h>
#include <raymath.h>

#include "guppy.h"

Color guppyColor(float x) {
    x = Remap(x, 0.0f, 1.0f, 1.0f, 0.0f);
    const int idx = std::clamp(static_cast<int>(x * 255.0f + 0.5f), 0, 255);
    const auto& rgb = guppy_colormap[idx];
    return Color{rgb[0], rgb[1], rgb[2], 255};
}