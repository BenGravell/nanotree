#pragma once

#include <raylib.h>

#include "planner/cost.h"
#include "planner/node.h"
#include "planner/path.h"
#include "planner/tree.h"
#include "ui/colors.h"

float normalizeCost(const float c, const float c_root, const float c_path, const float c_tree) {
    float x = 0.0f;
    if (c < c_path) {
        x = Remap((c - c_root) / (c_path - c_root), 0.0f, 1.0f, 0.0f, 0.5f);
    } else {
        x = Remap((c - c_path) / (c_tree - c_path), 0.0f, 1.0f, 0.5f, 1.0f);
    }
    return std::clamp(x, 0.0f, 1.0f);
}

void DrawTree(const Tree& tree, const Path& path, const Vector2 goal) {
    float cost_root = computeHeuristicCost(tree.nodes.front(), goal);

    float cost_path = 0.0f;
    for (const NodePtr& node : path) {
        cost_path = std::max(cost_path, computeHeuristicCost(node, goal));
    }

    float cost_tree = 0.0f;
    for (const NodePtr& node : tree.nodes) {
        cost_tree = std::max(cost_tree, computeHeuristicCost(node, goal));
    }

    // Sort by heuristic cost
    Nodes sorted_nodes = tree.nodes;
    std::sort(sorted_nodes.begin(), sorted_nodes.end(), TargetCostComparatorInv{goal});

    // Draw in sorted order
    for (const NodePtr& node : sorted_nodes) {
        if (!node->parent) {
            continue;
        }
        const Color color = computeCostColor(normalizeCost(computeHeuristicCost(node, goal), cost_root, cost_path, cost_tree));
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_TREE, color);
    }
}