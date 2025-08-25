#pragma once

#include <raylib.h>

#include <cmath>

#include "config.h"

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
    DrawCircleV(pos, params.radius, Fade(GRAY, 0.4f));
    DrawRing(pos, ring_inner_radius, ring_outer_radius, 0, 360, 0, Fade(GRAY, 0.6f));
    for (int i = 0; i < params.num_segments; ++i) {
        const float start_angle = 2 * i * delta_angle + offset_angle;
        DrawRing(pos, ring_inner_radius, ring_outer_radius, start_angle, start_angle + delta_angle, 0, LIGHTGRAY);
    }
}
