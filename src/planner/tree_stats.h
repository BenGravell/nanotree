#pragma once

#include "planner/path.h"

struct TreeStats {
  int num_nodes = 0;
  int num_path_nodes = 0;
  int num_nodes_lo_cost = 0;
  int num_nodes_hi_cost = 0;
  float cost_root = 0.0f;
  float cost_path_max = 0.0f;
  float cost_tree_max = 0.0f;
  float path_cost_to_come = 0.0f;
  float path_cost_to_go = 0.0f;
};

TreeStats computeTreeStats(const Nodes &nodes, const Path &path, const Vector2 goal) {
  TreeStats stats;
  stats.num_nodes = nodes.size();
  stats.num_path_nodes = path.size();
  stats.cost_root = nodes.front()->estimateCostTo(goal);
  stats.path_cost_to_come = path.back()->cost_to_come;
  stats.path_cost_to_go = computeCost(path.back()->pos, goal);

  const float path_cost = stats.path_cost_to_come + stats.path_cost_to_go;
  for (const NodePtr &node : nodes) {
    const float cost = node->estimateCostTo(goal);
    stats.cost_tree_max = std::max(stats.cost_tree_max, cost);
    if (cost < path_cost) {
      ++stats.num_nodes_lo_cost;
    } else {
      ++stats.num_nodes_hi_cost;
    }
  }
  for (const NodePtr &node : path) {
    stats.cost_path_max = std::max(stats.cost_path_max, node->estimateCostTo(goal));
  }
  return stats;
}
