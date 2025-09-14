#pragma once

#include <raylib.h>
#include <raymath.h>

#include "config.h"

bool isStartChanged(const Vector2 previous, const Vector2 current) {
    return Vector2DistanceSqr(previous, current) > START_CHANGED_DIST_MIN_SQR;
}

bool insideEnvironment(const Vector2 pos) {
    return (ENVIRONMENT_X_MIN < pos.x) && (pos.x < ENVIRONMENT_X_MAX) && (ENVIRONMENT_Y_MIN < pos.y) && (pos.y < ENVIRONMENT_Y_MAX);
}

Vector2 clampToEnvironment(const Vector2 pos) {
    return Vector2Clamp(pos, {ENVIRONMENT_X_MIN, ENVIRONMENT_Y_MIN}, {ENVIRONMENT_X_MAX, ENVIRONMENT_Y_MAX});
}

int snapToGridCenter(const float x, const int s) {
    const int i = std::lround(x);
    return i - (i % s) + (s / 2);
}

int snapToGridEdge(const float x, const int s) {
    const int i = std::lround(x);
    const int r = i % s;
    return (r < (s / 2)) ? (i - r) : (i - r + s);
}