#pragma once

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <vector>

#include "config.h"

using Obstacle = Vector2;
using Obstacles = std::vector<Obstacle>;

inline bool collides(const Vector2 pos, const Obstacle obstacle) {
    return Vector2DistanceSqr(obstacle, pos) < OBSTACLE_RADIUS_SQR;
}

inline bool collides(const Vector2 pos, const Obstacles obstacles) {
    return std::any_of(obstacles.begin(), obstacles.end(), [&pos](auto& obs) { return collides(pos, obs); });
}

inline bool collides(const Vector2 start, const Vector2 goal, const Obstacles& obstacles) {
    static constexpr float LERP_DEN = NUM_INTERMEDIATE_COLLISION_CHECK_POINTS - 1;
    for (int i = 0; i < NUM_INTERMEDIATE_COLLISION_CHECK_POINTS; ++i) {
        const float t = float(i) / LERP_DEN;
        if (collides(Vector2Lerp(start, goal, t), obstacles)) {
            return true;
        }
    }
    return false;
}
