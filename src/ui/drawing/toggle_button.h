#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"
#include "ui/elements/toggle_button.h"
#include "ui/drawing/shapes.h"

void DrawToggleButton(const ToggleButton& b) {
    DrawRectangleRec(b.rec, b.engaged ? COLOR_BUTTON_BACKGROUND_ACTIVE : COLOR_BUTTON_BACKGROUND_INACTIVE);
    DrawRectangleLinesEx(b.rec, 3.0f, b.engaged ? COLOR_BUTTON_BORDER_ACTIVE : COLOR_BUTTON_BORDER_INACTIVE);
    DrawScrollWheel(b.rec, b.engaged ? COLOR_BUTTON_CONTENT_ACTIVE : COLOR_BUTTON_CONTENT_INACTIVE, b.engaged ? 5.0f : 3.0f);
}
