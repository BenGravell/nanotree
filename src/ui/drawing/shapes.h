#pragma once

#include <raylib.h>

#include <algorithm>
#include <iterator>

void DrawChevron(const Rectangle &rec, const int dir, const Color color, const float scale = 1.0f) {
  const bool faces_right = dir > 0;
  const Vector2 center = {rec.x + 0.5f * rec.width, rec.y + 0.5f * rec.height};
  const float half_size = 0.5f * (scale * std::min(rec.height, rec.width));
  const float offset_x = faces_right ? half_size : -half_size;

  const Vector2 outline[] = {
      {center.x - offset_x, center.y - half_size},
      {center.x, center.y - half_size},
      {center.x + offset_x, center.y},
      {center.x, center.y + half_size},
      {center.x - offset_x, center.y + half_size},
  };

  for (std::size_t i = 0; (i + 1) < std::size(outline); ++i) {
    // Mirroring reverses the winding, so swap vertices for right-facing
    // triangles.
    const Vector2 a = faces_right ? center : outline[i];
    const Vector2 b = outline[i + 1];
    const Vector2 c = faces_right ? outline[i] : center;

    DrawTriangle(a, b, c, color);
  }
}
