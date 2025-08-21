#pragma once

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <vector>

#include "core/config.h"

using Obstacle = Vector2;
using Obstacles = std::vector<Obstacle>;

inline bool collides(const Vector2 pos, const Obstacle obstacle) {
    return Vector2Distance(obstacle, pos) < OBSTACLE_RADIUS;
}

inline bool collides(const Vector2 pos, const Obstacles obstacles) {
    return std::any_of(obstacles.begin(), obstacles.end(), [&pos](auto& obs) { return collides(pos, obs); });
}
