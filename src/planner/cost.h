#pragma once

#include <raylib.h>
#include <raymath.h>

#include "planner/node.h"

float computeCost(const Vector2 a, const Vector2 b) {
    return Vector2Distance(a, b);
}

// TODO rename to estimateThroughCostToGoal
// TODO make this a method of Node
float computeHeuristicCost(const NodePtr& node, const Vector2 goal) {
    return node->cost_to_come + computeCost(node->pos, goal);
}
