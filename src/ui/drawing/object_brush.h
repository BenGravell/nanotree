#pragma once

#include <raylib.h>

#include <cmath>
#include <stdexcept>

#include "config.h"
#include "core/problem/problem_edit_mode.h"
#include "ui/colors.h"

struct DrawObjectBrushParams {
  float radius;
  float ring_width_frac;
  int num_segments;
  float period_sec;
};

void DrawObjectBrush(const Vector2 pos, const DrawObjectBrushParams params) {
  const float delta_angle = 360.0f / (2.0f * params.num_segments);
  const float ring_outer_radius = params.radius;
  const float ring_width = params.ring_width_frac * params.radius;
  const float ring_inner_radius = ring_outer_radius - ring_width;
  const float current_time = GetTime();
  const float offset_angle = std::fmod(current_time / params.period_sec, 1.0f) * 360.0f;

  DrawCircleV(pos, params.radius, COLOR_OBJECT_BRUSH_BKGD);
  DrawRing(pos, ring_inner_radius, ring_outer_radius, 0, 360, 0, COLOR_OBJECT_BRUSH_FRGD);

  for (int i = 0; i < params.num_segments; ++i) {
    const float start_angle = 2 * i * delta_angle + offset_angle;
    DrawRing(pos, ring_inner_radius, ring_outer_radius, start_angle, start_angle + delta_angle, 0, LIGHTGRAY);
  }
}

DrawObjectBrushParams getObjectBrushParams(const ProblemEditMode mode) {
  switch (mode) {
    case ProblemEditMode::PLACE_START:
      return {START_RADIUS, 0.4f, 8, 4.0f};
    case ProblemEditMode::PLACE_GOAL:
      return {GOAL_RADIUS, 0.4f, 8, 4.0f};
    case ProblemEditMode::ADD_OBSTACLE:
      return {OBSTACLE_RADIUS, 0.3f, 12, 5.0f};
    case ProblemEditMode::DEL_OBSTACLE:
      return {OBSTACLE_DELETE_RADIUS, 0.5f, 6, 3.0f};
    default:
      throw std::logic_error("Unhandled ProblemEditMode");
  }
}
