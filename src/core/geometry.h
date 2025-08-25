#pragma once

#include <raylib.h>
#include <raymath.h>

#include "config.h"

bool insideEnvironment(const Vector2 pos) {
    return (ENVIRONMENT_X_MIN < pos.x) && (pos.x < ENVIRONMENT_X_MAX) && (ENVIRONMENT_Y_MIN < pos.y) && (pos.y < ENVIRONMENT_Y_MAX);
}

Vector2 clampToEnvironment(const Vector2 pos) {
    return Vector2Clamp(pos, {ENVIRONMENT_X_MIN, ENVIRONMENT_Y_MIN}, {ENVIRONMENT_X_MAX, ENVIRONMENT_Y_MAX});
}