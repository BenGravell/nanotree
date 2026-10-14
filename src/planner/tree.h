#pragma once

#include <functional>
#include <limits>
#include <unordered_map>

#include "planner/path.h"
#include "planner/sampling.h"
#include "planner/steering.h"

using ChildMap = std::unordered_map<NodePtr, std::unordered_set<NodePtr>>;

inline ChildMap buildChildMap(const Nodes &nodes) {
  ChildMap child_map;
  for (const NodePtr &node : nodes) {
    if (node->parent) {
      child_map[node->parent].insert(node);
    }
  }
  return child_map;
}

struct Tree {
  Nodes nodes;
  ChildMap child_map;

  Nodes getNear(const Vector2 target) const {
    std::vector<NodePtr> near_nodes;
    for (const NodePtr &node : nodes) {
      if (goalReached(node, target)) {
        near_nodes.push_back(node);
      }
    }
    return near_nodes;
  }
  void reset(const Vector2 start) {
    nodes = {std::make_shared<Node>(Node{nullptr, start, 0.0f})};
    child_map = buildChildMap(nodes);
  }
  void resetRoot(const Problem &problem, const Path &path) {
    // Special case: tree has zero or one node(s), just reset the whole tree.
    if (nodes.size() <= 1) {
      reset(problem.start);
      return;
    }

    // Collect the target nodes.
    // Target is goal region if any node reaches goal,
    // otherwise use path end.
    Nodes target_nodes = getNear(problem.goal);
    if ((target_nodes.size() == 0) && (path.size() > 0)) {
      target_nodes = {path.back()};
    }

    const auto cheapest_down = targetCosts(target_nodes);

    Nodes retained_nodes;

    // Always create a fresh root at start.
    NodePtr new_root = std::make_shared<Node>(Node{nullptr, problem.start, 0.0f});
    retained_nodes.push_back(new_root);

    const NodePtr best_child = selectRootChild(problem, cheapest_down);

    // Retain only the subtree rooted at best_child (if any),
    // re-parent it to the new root,
    // and update costs throughout that subtree.
    if (best_child) {
      // Re-parent attachment point and set its new cost_to_come.
      best_child->parent = new_root;
      best_child->cost_to_come = new_root->estimateCostTo(best_child->pos);
      retained_nodes.push_back(best_child);

      collectDescendants(best_child, retained_nodes);
    }

    nodes = std::move(retained_nodes);
    child_map = buildChildMap(nodes);
    updateSubtreeCosts(new_root);
  }
  void carry(const Path path, const int num_carry, const Obstacles &obstacles) {
    Nodes retained_nodes;
    NodeSet retained_set;

    // Ensure root is retained at the front.
    NodePtr root = nodes.front();
    retained_nodes.push_back(root);
    retained_set.insert(root);

    // Retain path.
    if (!path.empty()) {
      for (const NodePtr &node_add : path) {
        if (retained_nodes.size() > num_carry) {
          break;
        }

        if (retained_set.find(node_add) == retained_set.end()) {
          retained_nodes.push_back(node_add);
          retained_set.insert(node_add);
        }
      }
    }

    // Retain random nodes & all their ancestors.
    Nodes shuffled = nodes;
    std::shuffle(shuffled.begin(), shuffled.end(), rng);
    for (const NodePtr &node : shuffled) {
      if (retained_nodes.size() > num_carry) {
        break;
      }

      std::shared_ptr<Node> node_add = node;
      while (node_add) {
        if (retained_set.find(node_add) == retained_set.end()) {
          retained_nodes.push_back(node_add);
          retained_set.insert(node_add);
        }
        node_add = node_add->parent;
      }
    }

    nodes = std::move(retained_nodes);
    child_map = buildChildMap(nodes);
  }
  void cullByObstacles(const Obstacles &obstacles) {
    Nodes retained_nodes;

    // Ensure root is retained at the front.
    NodePtr root = nodes.front();
    retained_nodes.push_back(root);

    // Traverse from root and keep only collision-free nodes and subtrees.
    std::function<void(const NodePtr &)> dfs = [&](const NodePtr &node) {
      if (edgeCollides(node, obstacles)) {
        // Prune entire subtree.
        return;
      }

      // Keep this node.
      if (node != root) {
        retained_nodes.push_back(node);
      }

      // Recurse on children.
      auto it = child_map.find(node);
      if (it != child_map.end()) {
        for (const NodePtr &child : it->second) {
          dfs(child);
        }
      }
    };

    dfs(root);

    nodes = std::move(retained_nodes);
    child_map = buildChildMap(nodes);
  }
  void growOnce(Vector2 pos, const Obstacles &obstacles, const bool rewire_enabled) {
    NodePtr parent = getParent(pos, nodes, obstacles, REWIRE_RADIUS);

    pos = clampToEnvironment(pos);
    pos = attractByDistance(pos, parent);
    pos = attractByAngle(pos, parent);

    if (!insideEnvironment(pos)) {
      return;
    }

    if (collides(parent->pos, pos, obstacles)) {
      return;
    }

    const float cost_to_come = parent->estimateCostTo(pos);
    NodePtr node = std::make_shared<Node>(Node{parent, pos, cost_to_come});
    nodes.push_back(node);
    child_map[node->parent].insert(node);

    if (rewire_enabled) {
      rewire(node, obstacles);
    }
  }
  void rewire(const NodePtr &new_node, const Obstacles &obstacles) {
    for (NodePtr &neighbor : nodes) {
      if (neighbor == new_node || neighbor == new_node->parent) {
        continue;
      }

      const float cost = computeCost(new_node->pos, neighbor->pos);
      if (cost > REWIRE_RADIUS) {
        continue;
      }

      const float new_cost_to_come_of_neighbor = new_node->cost_to_come + cost;
      const bool cost_improved = new_cost_to_come_of_neighbor < neighbor->cost_to_come;
      if (cost_improved) {
        if (collides(new_node->pos, neighbor->pos, obstacles)) {
          continue;
        }

        child_map[neighbor->parent].erase(neighbor);
        neighbor->parent = new_node;
        neighbor->cost_to_come = new_cost_to_come_of_neighbor;
        child_map[new_node].insert(neighbor);
        updateSubtreeCosts(neighbor);
      }
    }
  }
  void updateSubtreeCosts(const NodePtr &node) {
    std::function<void(const NodePtr &)> dfs = [&](const NodePtr &current) {
      auto it = child_map.find(current);
      if (it != child_map.end()) {
        for (const NodePtr &child : it->second) {
          child->cost_to_come = current->estimateCostTo(child->pos);
          dfs(child);
        }
      }
    };

    dfs(node);
  }
  void grow(const Problem &problem, const int num_samples, const bool rewire_enabled) {
    for (int i = 0; i < num_samples; ++i) {
      const Vector2 pos = sample(problem.goal);
      growOnce(pos, problem.obstacles, rewire_enabled);
    }
  }

 private:
  using DownstreamCosts = std::unordered_map<NodePtr, float>;

  static DownstreamCosts targetCosts(const Nodes &target_nodes) {
    // For every ancestor on any target-reaching path, record the cheapest
    // downstream cost to a target along the existing tree.
    DownstreamCosts cheapest_down;
    for (const NodePtr &target_node : target_nodes) {
      NodePtr cur = target_node;
      while (cur) {
        const float delta = target_node->cost_to_come - cur->cost_to_come;
        auto it = cheapest_down.find(cur);
        if (it == cheapest_down.end() || delta < it->second) {
          cheapest_down[cur] = delta;
        }
        cur = cur->parent;
      }
    }

    return cheapest_down;
  }
  NodePtr selectRootChild(const Problem &problem, const DownstreamCosts &cheapest_down) const {
    // Choose the child of the new root.
    NodePtr best_child = nullptr;
    float best_total = std::numeric_limits<float>::infinity();

    if (!cheapest_down.empty()) {
      for (const auto &kv : cheapest_down) {
        const NodePtr &cand = kv.first;
        if (Vector2Distance(problem.start, cand->pos) > DEVIATION_DISTANCE_MAX) {
          continue;
        }
        if (collides(problem.start, cand->pos, problem.obstacles)) {
          continue;
        }

        const float down_cost = kv.second;
        const float up_cost = computeCost(problem.start, cand->pos);
        const float total = up_cost + down_cost;
        if (total < best_total) {
          best_total = total;
          best_child = cand;
        }
      }
    } else if (!nodes.empty()) {
      // No node in the current tree reaches the target;
      // fall back to attaching the nearest node.
      NodePtr cand = getNearest(problem.start, nodes);
      if (!(Vector2Distance(problem.start, cand->pos) > DEVIATION_DISTANCE_MAX) &&
          !collides(problem.start, cand->pos, problem.obstacles)) {
        best_child = cand;
      }
    }

    return best_child;
  }
  void collectDescendants(const NodePtr &node, Nodes &retained_nodes) const {
    auto it = child_map.find(node);
    if (it != child_map.end()) {
      for (const NodePtr &child : it->second) {
        retained_nodes.push_back(child);
        collectDescendants(child, retained_nodes);
      }
    }
  }
};
