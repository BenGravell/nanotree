#pragma once

#include "core/geometry.h"
#include "core/rng.h"

Vector2 sampleNearGoal(const Vector2 goal) {
  static std::uniform_real_distribution<float> dist_goal_r(0, GOAL_RADIUS);
  static std::uniform_real_distribution<float> dist_goal_t(0.0f, 2.0f * M_PI);
  const float r = dist_goal_r(rng);
  const float t = dist_goal_t(rng);
  const Vector2 delta = {r * std::cos(t), r * std::sin(t)};
  return clampToEnvironment(goal + delta);
}

Vector2 sampleEnv() {
  static std::uniform_real_distribution<float> dist_x(ENVIRONMENT_X_MIN, ENVIRONMENT_X_MAX);
  static std::uniform_real_distribution<float> dist_y(ENVIRONMENT_Y_MIN, ENVIRONMENT_Y_MAX);
  return Vector2{dist_x(rng), dist_y(rng)};
}

Vector2 sample(const Vector2 goal) {
  static std::uniform_real_distribution<float> dist_select(0.0f, 1.0f);
  return (dist_select(rng) < GOAL_SAMPLE_PROBABILITY) ? sampleNearGoal(goal) : sampleEnv();
}
