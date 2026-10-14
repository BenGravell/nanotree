#pragma once

#include <raylib.h>

#include "core/geometry.h"

struct MouseState {
  Vector2 pos;
  bool inside;
  bool down;
};

MouseState getMouseState(const bool snap) {
  const Vector2 mouse_pos = GetMousePosition();
  Vector2 pos = clampToEnvironment(mouse_pos);
  if (snap) {
    pos = snapToGridCenter(pos, CELL_SIZE);
  }
  return {pos, insideEnvironment(mouse_pos), IsMouseButtonDown(MOUSE_BUTTON_LEFT)};
}
