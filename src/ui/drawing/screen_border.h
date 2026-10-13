#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"

void DrawScreenBorder() {
    DrawRectangleLinesEx(SCREEN_REC, BORDER_THICKNESS, COLOR_SCREEN_BORDER);
}
