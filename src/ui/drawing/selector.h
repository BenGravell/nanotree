#pragma once

#include <raylib.h>

#include <string>
#include <vector>

#include "config.h"
#include "ui/colors.h"
#include "ui/elements/selector.h"

void DrawSelector(const Selector& selector, const Font font) {
    const std::map<SelectorMode, std::string> label_map = {
        {SelectorMode::PLACE_START, "Place start"},
        {SelectorMode::PLACE_GOAL, "Place goal"},
        {SelectorMode::DEL_OBSTACLE, "- Obstacle"},
        {SelectorMode::ADD_OBSTACLE, "+ Obstacle"},
    };

    for (const auto& [mode, rec] : selector.rectangles) {
        const bool active = (mode == selector.mode);
        const Color fill = active ? COLOR_BUTTON_BACKGROUND_ACTIVE : COLOR_BUTTON_BACKGROUND_INACTIVE;
        const Color border = active ? COLOR_BUTTON_BORDER_ACTIVE : COLOR_BUTTON_BORDER_INACTIVE;
        const Color text = active ? COLOR_BUTTON_CONTENT_ACTIVE : COLOR_BUTTON_CONTENT_INACTIVE;

        DrawRectangleRec(rec, fill);
        DrawRectangleLinesEx(rec, 3, border);

        auto it = label_map.find(mode);
        if (it != label_map.end()) {
            const Vector2 textSize = MeasureTextEx(font, it->second.c_str(), TEXT_HEIGHT_CONTROL, 1);
            const Vector2 textPos = {
                rec.x + 0.5f * (rec.width - textSize.x),
                rec.y + 0.5f * (rec.height - textSize.y)};
            DrawTextEx(font, it->second.c_str(), textPos, TEXT_HEIGHT_CONTROL, 1, text);
        }
    }
}