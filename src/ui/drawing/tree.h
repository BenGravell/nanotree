#pragma once

#include <raylib.h>

#include "planner/cost.h"
#include "planner/node.h"
#include "planner/path.h"
#include "planner/tree.h"
#include "ui/colors.h"

float normalizeCost(const float cost, const float cost_root, const float cost_path, const float cost_tree) {
    float x = 0.0f;
    if (cost < cost_path) {
        x = Remap((cost - cost_root) / (cost_path - cost_root), 0.0f, 1.0f, 0.0f, 0.5f);
    } else {
        x = Remap((cost - cost_path) / (cost_tree - cost_path), 0.0f, 1.0f, 0.5f, 1.0f);
    }
    return std::clamp(x, 0.0f, 1.0f);
}

const float computeLineWidth(const int tree_size) {
    const int n = std::clamp(tree_size, LINE_WIDTH_TREE_SIZE_MIN, LINE_WIDTH_TREE_SIZE_MAX);
    return Remap(1.0f / n, TREE_SIZE_INV_MIN, TREE_SIZE_INV_MAX, LINE_WIDTH_TREE_MIN, LINE_WIDTH_TREE_MAX);
}

void DrawTree(const Tree& tree, const Path& path, const Vector2 goal, const bool goal_reached) {
    float cost_root = computeHeuristicCost(tree.nodes.front(), goal);

    float cost_path = 0.0f;
    for (const NodePtr& node : path) {
        cost_path = std::max(cost_path, computeHeuristicCost(node, goal));
    }

    float cost_tree = 0.0f;
    for (const NodePtr& node : tree.nodes) {
        cost_tree = std::max(cost_tree, computeHeuristicCost(node, goal));
    }

    // TODO check if this is hurting runtime performance...
    // Sort by heuristic cost
    Nodes sorted_nodes = tree.nodes;
    std::sort(sorted_nodes.begin(), sorted_nodes.end(), TargetCostComparatorInv{goal});

    // Draw in sorted order.
    const float line_width = computeLineWidth(tree.nodes.size());
    for (const NodePtr& node : sorted_nodes) {
        if (!node->parent) {
            continue;
        }
        const Color color = computeCostColor(normalizeCost(computeHeuristicCost(node, goal), cost_root, cost_path, cost_tree), goal_reached);
        DrawLineEx(node->parent->pos, node->pos, line_width, color);
    }
}