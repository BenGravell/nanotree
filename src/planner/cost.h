#pragma once

#include <raylib.h>
#include <raymath.h>

float computeCost(const Vector2 a, const Vector2 b) {
    return Vector2Distance(a, b);
}
