#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"

Color computeGoalColor(const bool goal_reached) {
    return goal_reached ? COLOR_GOAL_REACHED : COLOR_GOAL_NOT_REACHED;
}

void DrawStart(const Vector2 start) {
    DrawCircleV(start, START_RADIUS, Fade(COLOR_START, 0.8f));
}

void DrawGoal(const Vector2 goal, const bool goal_reached) {
    DrawCircleV(goal, GOAL_RADIUS, Fade(computeGoalColor(goal_reached), 0.8f));
}
