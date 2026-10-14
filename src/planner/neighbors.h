#pragma once

#include <algorithm>

#include "planner/collision.h"

struct TargetDistanceComparator {
  Vector2 target;

  bool operator()(const NodePtr &a, const NodePtr &b) const {
    return Vector2DistanceSqr(a->pos, target) < Vector2DistanceSqr(b->pos, target);
  }
};

struct TargetCostComparator {
  Vector2 target;

  bool operator()(const NodePtr &a, const NodePtr &b) const {
    return a->estimateCostTo(target) < b->estimateCostTo(target);
  }
};

struct TargetCostComparatorInv {
  Vector2 target;

  bool operator()(const NodePtr &a, const NodePtr &b) const {
    return a->estimateCostTo(target) > b->estimateCostTo(target);
  }
};

NodePtr getNearest(const Vector2 target, const Nodes &nodes) {
  return *std::min_element(nodes.begin(), nodes.end(), TargetDistanceComparator{target});
}

NodePtr getCheapest(const Vector2 target, const Nodes &nodes) {
  return *std::min_element(nodes.begin(), nodes.end(), TargetCostComparator{target});
}

Nodes getNeighbors(const Vector2 target, const Nodes &nodes, const Obstacles &obstacles, const float max_dist) {
  Nodes neighbors;
  for (const NodePtr &node : nodes) {
    const float dist = Vector2Distance(node->pos, target);
    if (dist > max_dist) {
      continue;
    }

    const bool in_collision = (dist < MAX_DISTANCE_BETWEEN_POSES_FOR_COLLISION_CHECK)
                                  ? collides(node->pos, obstacles)
                                  : collides(node->pos, target, obstacles);
    if (in_collision) {
      continue;
    }

    neighbors.push_back(node);
  }
  return neighbors;
}

NodePtr getParent(const Vector2 target, const Nodes &nodes, const Obstacles &obstacles, const float max_dist) {
  const Nodes neighbors = getNeighbors(target, nodes, obstacles, max_dist);

  if (neighbors.empty()) {
    return getNearest(target, nodes);
  }

  return getCheapest(target, neighbors);
}
