#pragma once

#include <algorithm>

#include "core/geometry.h"
#include "planner/node.h"

Vector2 attractByDistance(const Vector2 pos, const NodePtr parent) {
  const Vector2 direction = Vector2Normalize(pos - parent->pos);
  const float distance = std::min(DEVIATION_DISTANCE_MAX, Vector2Distance(parent->pos, pos));
  return Vector2Add(parent->pos, direction * distance);
}

Vector2 attractByAngle(const Vector2 pos, const NodePtr parent) {
  const Vector2 x = parent->parent ? parent->parent->pos : parent->pos - (pos - parent->pos);
  const Vector2 y = parent->pos;
  const Vector2 z = pos;
  const Vector2 direction_yz = Vector2Normalize(z - y);
  const Vector2 direction_xy = Vector2Normalize(y - x);
  const float distance_yz = Vector2Distance(y, z);
  const float distance_xy = Vector2Distance(x, y);
  const float deviation_angle_max =
      std::asin(std::clamp(0.5f * (distance_xy + distance_yz) / RADIUS_OF_CURVATURE_MIN, 0.0f, 1.0f));
  const float deviation_angle =
      std::clamp(Vector2Angle(direction_xy, direction_yz), -deviation_angle_max, deviation_angle_max);
  const Vector2 direction_out = Vector2Rotate(direction_xy, deviation_angle);
  return Vector2Add(parent->pos, direction_out * distance_yz);
}
