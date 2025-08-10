#pragma once

#include <raylib.h>
#include <raymath.h>

#include "config.h"


bool insideEnvironment(const Vector2 pos) {
    return (0.0f < pos.x) && (pos.x < ENVIRONMENT_WIDTH) && (0.0f < pos.y) && (pos.y < ENVIRONMENT_HEIGHT);
}

Vector2 clampToEnvironment(const Vector2 pos) {
    return Vector2Clamp(pos, {0, 0}, {ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT});
}