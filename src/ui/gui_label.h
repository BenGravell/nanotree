#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"

void GuiLabelColor(const Rectangle bounds, const char* text, const Color color) {
    const int color_original = GuiGetStyle(LABEL, TEXT_COLOR_NORMAL);

    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(color));
    GuiLabel(bounds, text);

    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, color_original);
}

void GuiLabelValue(const Rectangle bounds, const char* label_text, const char* value_text) {
    const int text_alignment_original = GuiGetStyle(DEFAULT, TEXT_ALIGNMENT);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiLabel(bounds, label_text);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabel(bounds, value_text);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, text_alignment_original);
}

void GuiLabelValueColor(const Rectangle bounds, const char* label_text, const char* value_text, const Color color) {
    const int text_alignment_original = GuiGetStyle(DEFAULT, TEXT_ALIGNMENT);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiLabelColor(bounds, label_text, color);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabelColor(bounds, value_text, color);

    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, text_alignment_original);
}

void GuiLabelTimingStat(const Rectangle bounds, const char* label_text, const float duration, const bool major) {
    const char* value_text = TextFormat("%4d ms", int(1000.0f * duration));
    const Color color = major ? computeFrameTimeColor(duration) : COLOR_MINOR_STAT;
    GuiLabelValueColor(bounds, label_text, value_text, color);
}

template <size_t N>
void GuiLabelSpinner(Rectangle bounds, const char* label_text, int* ix, std::array<int, N> options) {
    GuiSpinner(bounds, NULL, ix, 0, N - 1, false);
    const Rectangle label_bounds = {bounds.x, bounds.y + bounds.height + BUTTON_SPACING_Y / 2, bounds.width, TEXT_HEIGHT};
    GuiLabelValue(label_bounds, label_text, TextFormat("%6d", options[*ix]));
}