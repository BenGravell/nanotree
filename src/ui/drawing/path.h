#pragma once

#include <raylib.h>

#include "config.h"
#include "planner/node.h"
#include "planner/path.h"
#include "ui/colors.h"

void DrawPath(const Path& path, const bool goal_reached) {
    for (const NodePtr& node : path) {
        if (!node->parent) {
            continue;
        }
        const Color color = goal_reached ? COLOR_PATH_GOAL_REACHED : COLOR_PATH_GOAL_NOT_REACHED;
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_PATH, color);
        DrawCircleV(node->pos, 0.5f * NODE_WIDTH_PATH, color);
    }
}
