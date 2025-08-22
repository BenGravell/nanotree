#pragma once

#include <raylib.h>

#include "config.h"
#include "planner/node.h"
#include "planner/path.h"
#include "ui/colors.h"

void DrawPath(const Path& path) {
    for (const NodePtr& node : path) {
        if (!node->parent) {
            continue;
        }
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_PATH, COLOR_PATH);
        DrawCircleV(node->pos, 0.5f * NODE_WIDTH_PATH, COLOR_PATH);
    }
}
