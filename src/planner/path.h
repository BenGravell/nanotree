#pragma once

#include <raylib.h>

#include "core/problem/problem.h"
#include "planner/neighbors.h"

using Path = std::vector<NodePtr>;

Path extractPath(const Nodes& nodes, const Problem& problem) {
    Path path;
    NodePtr node = getParent(problem.goal, nodes, problem.obstacles, GOAL_RADIUS);
    while (node->parent) {
        path.push_back(node);
        node = node->parent;
    }
    path.push_back(node);
    std::reverse(path.begin(), path.end());
    return path;
}

bool goalReached(const NodePtr node, const Vector2 goal) {
    // TODO use Vector2DistanceSqr and a squared goal radius.
    return Vector2Distance(node->pos, goal) < GOAL_RADIUS;
}

bool goalReached(const Path& path, const Vector2 goal) {
    return goalReached(path.back(), goal);
}
