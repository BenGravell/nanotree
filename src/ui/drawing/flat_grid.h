#pragma once

#include <raylib.h>

struct FlatGridParams {
    int spacing;
    int thickness;
    Color color;
};

void DrawFlatGrid(const int x_min, const int x_max, const int y_min, const int y_max, const FlatGridParams params) {
    for (int x = x_min; x <= x_max; x += params.spacing) {
        DrawLineEx({float(x), float(y_min)}, {float(x), float(y_max)}, params.thickness, params.color);
    }
    for (int y = y_min; y <= y_max; y += params.spacing) {
        DrawLineEx({float(x_min), float(y)}, {float(x_max), float(y)}, params.thickness, params.color);
    }
}