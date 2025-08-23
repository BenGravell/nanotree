#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"
#include "ui/elements/momentary_button.h"
#include "ui/drawing/shapes.h"

// TODO de-duplicate with ToggleButton, logic is nearly the same
void DrawMomentaryButton(const MomentaryButton& b) {
    DrawRectangleRec(b.rec, b.engaged ? COLOR_BUTTON_BACKGROUND_ACTIVE : COLOR_BUTTON_BACKGROUND_INACTIVE);
    DrawRectangleLinesEx(b.rec, 3.0f, b.engaged ? COLOR_BUTTON_BORDER_ACTIVE : COLOR_BUTTON_BORDER_INACTIVE);
    // TODO draw generic contents
}
