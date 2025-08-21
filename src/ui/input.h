#pragma once

#include <raylib.h>

int GetMouseWheelMoveSign() {
    const int scroll = GetMouseWheelMove();
    return (scroll > 0) - (scroll < 0);
}
