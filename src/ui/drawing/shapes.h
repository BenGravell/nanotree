#pragma once

#include <raylib.h>
#include <raymath.h>

#include <cmath>

void DrawSquare(const Vector2 center, const float radius, const Color color) {
    DrawRectangle(center.x - radius, center.y - radius, 2 * radius, 2 * radius, color);
}

void DrawChevron(const Rectangle& rec, const float dir, const Color color) {
    const Vector2 c = {rec.x + 0.5f * rec.width, rec.y + 0.5f * rec.height};
    const float s = 0.5f * 0.75f * std::min(rec.width, rec.height);

    Vector2 v[6];
    const float x_scale = std::abs(dir);

    // TODO make this less verbose
    if (dir > 0.0f) {
        // Right-facing outline (top-left -> mid-top -> tip -> mid-bot -> bot-left -> inner-notch)
        v[0] = {c.x - x_scale * s, c.y - s};  // top-left
        v[1] = {c.x, c.y - s};                // mid-top
        v[2] = {c.x + x_scale * s, c.y};      // tip (right)
        v[3] = {c.x, c.y + s};                // mid-bot
        v[4] = {c.x - x_scale * s, c.y + s};  // bot-left
        v[5] = {c.x - x_scale, c.y};          // inner notch (left of center)

        DrawTriangle(v[5], v[1], v[0], color);
        DrawTriangle(v[5], v[2], v[1], color);
        DrawTriangle(v[5], v[3], v[2], color);
        DrawTriangle(v[5], v[4], v[3], color);
    } else {
        // Left-facing outline (top-right -> mid-top -> tip -> mid-bot -> bot-right -> inner-notch)
        v[0] = {c.x + x_scale * s, c.y - s};  // top-right
        v[1] = {c.x, c.y - s};                // mid-top
        v[2] = {c.x - x_scale * s, c.y};      // tip (left)
        v[3] = {c.x, c.y + s};                // mid-bot
        v[4] = {c.x + x_scale * s, c.y + s};  // bot-right
        v[5] = {c.x + x_scale, c.y};          // inner notch (right of center)

        DrawTriangle(v[0], v[1], v[5], color);
        DrawTriangle(v[1], v[2], v[5], color);
        DrawTriangle(v[2], v[3], v[5], color);
        DrawTriangle(v[3], v[4], v[5], color);
    }
}

void DrawDoubleChevron(const Rectangle& rec, const float dir, const Color color) {
    const float offset = 0.18f * rec.width;
    const float mult = (dir > 0.0f) ? 1.0f : -1.0f;

    Rectangle rec1 = rec;
    Rectangle rec2 = rec;

    rec1.x += mult * offset;
    rec2.x -= mult * offset;

    DrawChevron(rec1, dir, color);
    DrawChevron(rec2, dir, color);
}

void DrawScrollWheel(Rectangle bounds, Color color, const float thickness) {
    const float center_x = bounds.x + 0.5f * bounds.width;
    const float center_y = bounds.y + 0.5f * bounds.height;

    // Dimensions relative to rectangle
    const float body_height = 0.7f * bounds.height;
    const float body_width = 0.3f * bounds.width;

    // Draw scroll wheel body (oval/rounded rectangle)
    Rectangle body = {
        center_x - 0.5f * body_width,
        center_y - 0.5f * body_height,
        body_width,
        body_height};
    DrawRectangleRoundedLinesEx(body, 1.0f, 16, thickness, color);

    // Draw horizontal bars inside wheel
    const int lines = 5;
    const float bar_width_min = 0.3f * body_width;
    const float bar_width_max = 0.7f * body_width;
    for (int i = 0; i < lines; i++) {
        const float t = (i + 1.0f) / (lines + 1.0f);
        const float y = body.y + t * body_height;
        const float bar_width = Lerp(bar_width_min, bar_width_max, (1.0f - 2.0f * std::abs(t - 0.5)));
        DrawLineEx({center_x - 0.5f * bar_width, y},
                   {center_x + 0.5f * bar_width, y},
                   thickness,
                   color);
    }
}