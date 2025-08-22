#pragma once

#include <raylib.h>

#include "config.h"
#include "core/obstacle.h"
#include "ui/colors.h"

void DrawObstacles(const Obstacles& obstacles) {
    for (const Vector2 obstacle : obstacles) {
        DrawCircleV(obstacle, OBSTACLE_RADIUS, COLOR_OBSTACLE);
    }
}
