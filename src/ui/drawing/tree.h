#pragma once

#include <raylib.h>

#include "planner/cost.h"
#include "planner/node.h"
#include "planner/path.h"
#include "planner/tree.h"
#include "ui/colors.h"

float computeLineWidth(const int tree_size) {
    const int n = std::clamp(tree_size, LINE_WIDTH_TREE_SIZE_MIN, LINE_WIDTH_TREE_SIZE_MAX);
    return Remap(1.0f / n, TREE_SIZE_INV_MIN, TREE_SIZE_INV_MAX, LINE_WIDTH_TREE_MIN, LINE_WIDTH_TREE_MAX);
}

float computeMaxCost(Nodes nodes, const Vector2 goal) {
    float cost_max = 0.0f;
    for (const NodePtr& node : nodes) {
        cost_max = std::max(cost_max, node->estimateCostTo(goal));
    }
    return cost_max;
}

float normalizeCost(const float cost, const float cost_root, const float cost_path, const float cost_tree) {
    float x = 0.0f;
    if (cost < cost_path) {
        x = Remap((cost - cost_root) / (cost_path - cost_root), 0.0f, 1.0f, 0.0f, 0.5f);
    } else {
        x = Remap((cost - cost_path) / (cost_tree - cost_path), 0.0f, 1.0f, 0.5f, 1.0f);
    }
    return std::clamp(x, 0.0f, 1.0f);
}

Color computeCostColor(const float x, const bool goal_reached) {
    if (goal_reached && (x > 0.5f)) {
        return COLOR_GRAY_064;
    }
    static constexpr double s = 0.3f;
    const double y = (x < 0.5f) ? Remap(x, 0.0f, 0.5f, 0.0f, s) : Remap(x, 0.5f, 1.0f, 1.0f - s, 1.0f);
    return guppyColor(y);
}

void DrawTree(const Tree& tree, const Path& path, const Vector2 goal, const bool goal_reached) {
    const NodePtr root = tree.nodes.front();
    const float cost_root = root->estimateCostTo(goal);
    const float cost_path = computeMaxCost(path, goal);
    const float cost_tree = computeMaxCost(tree.nodes, goal);

    // Sort by heuristic cost.
    Nodes sorted_nodes = tree.nodes;
    std::sort(sorted_nodes.begin(), sorted_nodes.end(), TargetCostComparatorInv{goal});

    // Draw in sorted order.
    const float line_width = computeLineWidth(tree.nodes.size());
    for (const NodePtr& node : sorted_nodes) {
        if (!node->parent) {
            continue;
        }
        const float cost = node->estimateCostTo(goal);
        const float cost_normalized = normalizeCost(cost, cost_root, cost_path, cost_tree);
        const Color color = computeCostColor(cost_normalized, goal_reached);
        DrawLineEx(node->parent->pos, node->pos, line_width, color);
    }
}