#pragma once

#include <raylib.h>

#include "ui/drawing/delta_button_group.h"
#include "ui/drawing/toggle_button.h"
#include "ui/elements/number_widget.h"

void DrawNumberWidget(const NumberWidget& w) {
    DrawDeltaButtonGroup(w.delta_button_group);
}
