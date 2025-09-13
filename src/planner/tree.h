#pragma once

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <random>

#include "core/geometry.h"
#include "core/obstacle.h"
#include "core/rng.h"
#include "planner/cost.h"
#include "planner/node.h"
#include "planner/path.h"

bool edgeCollides(const Vector2 start, const Vector2 goal, const Obstacles& obstacles) {
    static constexpr float LERP_DEN = NUM_INTERMEDIATE_COLLISION_CHECK_POINTS - 1;
    for (int i = 0; i < NUM_INTERMEDIATE_COLLISION_CHECK_POINTS; ++i) {
        const float t = float(i) / LERP_DEN;
        if (collides(Vector2Lerp(start, goal, t), obstacles)) {
            return true;
        }
    }
    return false;
}

bool edgeCollides(const NodePtr& node, const Obstacles& obstacles) {
    return (node->parent) ? edgeCollides(node->parent->pos, node->pos, obstacles) : collides(node->pos, obstacles);
}

struct TargetDistanceComparator {
    Vector2 target;

    bool operator()(const NodePtr& a, const NodePtr& b) const {
        return computeCost(a->pos, target) < computeCost(b->pos, target);
    }
};

struct TargetCostComparator {
    Vector2 target;

    bool operator()(const NodePtr& a, const NodePtr& b) const {
        return computeHeuristicCost(a, target) < computeHeuristicCost(b, target);
    }
};

struct TargetCostComparatorInv {
    Vector2 target;

    bool operator()(const NodePtr& a, const NodePtr& b) const {
        return computeHeuristicCost(a, target) > computeHeuristicCost(b, target);
    }
};

NodePtr getNearest(const Vector2 target, const Nodes& nodes) {
    return *std::min_element(nodes.begin(), nodes.end(), TargetDistanceComparator{target});
}

NodePtr getCheapest(const Vector2 target, const Nodes& nodes) {
    return *std::min_element(nodes.begin(), nodes.end(), TargetCostComparator{target});
}

NodePtr getCheapest(const Vector2 target, const Nodes& nodes, const Obstacles& obstacles, const float max_dist) {
    Nodes neighbors;
    for (const NodePtr& node : nodes) {
        const float dist = Vector2Distance(node->pos, target);
        if (dist > max_dist) {
            continue;
        }

        const bool in_collision = (dist < MAX_DISTANCE_BETWEEN_POSES_FOR_COLLISION_CHECK) ? collides(node->pos, obstacles) : edgeCollides(node->pos, target, obstacles);
        if (in_collision) {
            continue;
        }

        neighbors.push_back(node);
    }

    if (neighbors.empty()) {
        return getNearest(target, nodes);
    }

    return getCheapest(target, neighbors);
}

Path extractPath(const Vector2 target, const Nodes& nodes, const Obstacles& obstacles) {
    Path path;
    NodePtr node = getCheapest(target, nodes, obstacles, GOAL_RADIUS);
    while (node->parent) {
        path.push_back(node);
        node = node->parent;
    }
    path.push_back(node);
    std::reverse(path.begin(), path.end());
    return path;
}

Vector2 sampleNearGoal(const Vector2 goal) {
    static std::uniform_real_distribution<float> dist_goal_r(0, GOAL_RADIUS);
    static std::uniform_real_distribution<float> dist_goal_t(0.0f, 2.0f * M_PI);
    const float r = dist_goal_r(rng);
    const float t = dist_goal_t(rng);
    const Vector2 delta = {r * std::cos(t), r * std::sin(t)};
    return clampToEnvironment(goal + delta);
}

Vector2 sampleEnv() {
    static std::uniform_real_distribution<float> dist_x(ENVIRONMENT_X_MIN, ENVIRONMENT_X_MAX);
    static std::uniform_real_distribution<float> dist_y(ENVIRONMENT_Y_MIN, ENVIRONMENT_Y_MAX);
    return Vector2{dist_x(rng), dist_y(rng)};
}

Vector2 sample(const Vector2 goal) {
    static std::uniform_real_distribution<float> dist_select(0.0f, 1.0f);
    return (dist_select(rng) < GOAL_SAMPLE_PROBABILITY) ? sampleNearGoal(goal) : sampleEnv();
}

Vector2 attractByDistance(const Vector2 pos, const NodePtr parent) {
    const Vector2 direction = Vector2Normalize(pos - parent->pos);
    const float distance = std::min(DEVIATION_DISTANCE_MAX, Vector2Distance(parent->pos, pos));
    return Vector2Add(parent->pos, direction * distance);
}

Vector2 attractByAngle(const Vector2 pos, const NodePtr parent) {
    const Vector2 x = parent->parent ? parent->parent->pos : parent->pos - (pos - parent->pos);
    const Vector2 y = parent->pos;
    const Vector2 z = pos;
    const Vector2 direction_yz = Vector2Normalize(z - y);
    const Vector2 direction_xy = Vector2Normalize(y - x);
    const float distance_yz = Vector2Distance(y, z);
    const float distance_xy = Vector2Distance(x, y);
    const float deviation_angle_max = std::asin(std::clamp(0.5f * (distance_xy + distance_yz) / RADIUS_OF_CURVATURE_MIN, 0.0f, 1.0f));
    const float deviation_angle = std::clamp(Vector2Angle(direction_xy, direction_yz), -deviation_angle_max, deviation_angle_max);
    const Vector2 direction_out = Vector2Rotate(direction_xy, deviation_angle);
    return Vector2Add(parent->pos, direction_out * distance_yz);
}

bool goalReached(const NodePtr node, const Vector2 goal) {
    return Vector2Distance(node->pos, goal) < GOAL_RADIUS;
}

bool goalReached(const Path& path, const Vector2 goal) {
    return goalReached(path.back(), goal);
}

using ChildMap = std::unordered_map<NodePtr, std::unordered_set<NodePtr>>;

inline ChildMap buildChildMap(const Nodes& nodes) {
    ChildMap child_map;
    for (const NodePtr& node : nodes) {
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
        for (const NodePtr& node : nodes) {
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

    void resetRoot(const Vector2 start, const Vector2 goal, const Path& path, const Obstacles& obstacles) {
        // Special case: tree has zero or one node(s), just reset the whole tree.
        if (nodes.size() <= 1) {
            reset(start);
            return;
        }

        // Collect the target nodes.
        // Target is goal region if any node reaches goal,
        // otherwise use path end.
        Nodes target_nodes = getNear(goal);
        if ((target_nodes.size() == 0) && (path.size() > 0)) {
            target_nodes = {path.back()};
        }

        // For every ancestor on any target-reaching path, record the cheapest
        // downstream cost to a target along the existing tree.
        std::unordered_map<NodePtr, float> cheapest_down;
        for (const NodePtr& target_node : target_nodes) {
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

        Nodes retained_nodes;

        // Always create a fresh root at start.
        NodePtr new_root = std::make_shared<Node>(Node{nullptr, start, 0.0f});
        retained_nodes.push_back(new_root);

        // Choose the child of the new root.
        NodePtr best_child = nullptr;
        float best_total = std::numeric_limits<float>::infinity();

        if (!cheapest_down.empty()) {
            for (const auto& kv : cheapest_down) {
                const NodePtr& cand = kv.first;
                const float dist = Vector2Distance(start, cand->pos);
                if (dist > DEVIATION_DISTANCE_MAX) {
                    continue;
                }
                if (edgeCollides(start, cand->pos, obstacles)) {
                    continue;
                }

                const float down_cost = kv.second;
                const float up_cost = computeCost(start, cand->pos);
                const float total = up_cost + down_cost;
                if (total < best_total) {
                    best_total = total;
                    best_child = cand;
                }
            }
        } else if (!nodes.empty()) {
            // No node in the current tree reaches the target;
            // fall back to attaching the nearest node.
            NodePtr cand = getNearest(start, nodes);
            const float dist = Vector2Distance(start, cand->pos);
            if (!((dist > DEVIATION_DISTANCE_MAX) || edgeCollides(start, cand->pos, obstacles))) {
                best_child = cand;
            }
        }

        // Retain only the subtree rooted at best_child (if any),
        // re-parent it to the new root,
        // and update costs throughout that subtree.
        if (best_child) {
            // Re-parent attachment point and set its new cost_to_come.
            best_child->parent = new_root;
            best_child->cost_to_come = computeHeuristicCost(new_root, best_child->pos);
            retained_nodes.push_back(best_child);

            // Collect descendants of best_child using the current child_map.
            std::function<void(const NodePtr&)> dfs = [&](const NodePtr& n) {
                auto it = child_map.find(n);
                if (it != child_map.end()) {
                    for (const NodePtr& child : it->second) {
                        retained_nodes.push_back(child);
                        dfs(child);
                    }
                }
            };
            dfs(best_child);
        }

        nodes = std::move(retained_nodes);
        child_map = buildChildMap(nodes);
        updateSubtreeCosts(new_root);
    }

    void carry(const Path path, const int num_carry, const Obstacles& obstacles) {
        Nodes retained_nodes;
        NodeSet retained_set;

        // Ensure root is retained at the front.
        NodePtr root = nodes.front();
        retained_nodes.push_back(root);
        retained_set.insert(root);

        // Retain path.
        if (!path.empty()) {
            for (const NodePtr& node_add : path) {
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
        for (const NodePtr& node : shuffled) {
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

    void cullByObstacles(const Obstacles& obstacles) {
        Nodes retained_nodes;

        // Ensure root is retained at the front.
        NodePtr root = nodes.front();
        retained_nodes.push_back(root);

        // Traverse from root and keep only collision-free nodes and subtrees.
        std::function<void(const NodePtr&)> dfs = [&](const NodePtr& node) {
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
                for (const NodePtr& child : it->second) {
                    dfs(child);
                }
            }
        };

        dfs(root);

        nodes = std::move(retained_nodes);
        child_map = buildChildMap(nodes);
    }

    void growOnce(Vector2 pos, const Obstacles& obstacles, const bool rewire_enabled) {
        NodePtr parent = getCheapest(pos, nodes, obstacles, REWIRE_RADIUS);

        pos = clampToEnvironment(pos);
        pos = attractByDistance(pos, parent);
        pos = attractByAngle(pos, parent);

        if (!insideEnvironment(pos)) {
            return;
        }

        if (edgeCollides(parent->pos, pos, obstacles)) {
            return;
        }

        const float cost_to_come = computeHeuristicCost(parent, pos);
        NodePtr node = std::make_shared<Node>(Node{parent, pos, cost_to_come});
        nodes.push_back(node);
        child_map[node->parent].insert(node);

        if (rewire_enabled) {
            rewire(node, obstacles);
        }
    }

    void rewire(const NodePtr& new_node, const Obstacles& obstacles) {
        for (NodePtr& neighbor : nodes) {
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
                if (edgeCollides(new_node->pos, neighbor->pos, obstacles)) {
                    continue;
                }

                // TODO check that new edge honors attractByAngle constraint

                child_map[neighbor->parent].erase(neighbor);
                neighbor->parent = new_node;
                neighbor->cost_to_come = new_cost_to_come_of_neighbor;
                child_map[new_node].insert(neighbor);
                updateSubtreeCosts(neighbor);
            }
        }
    }

    void updateSubtreeCosts(const NodePtr& node) {
        std::function<void(const NodePtr&)> dfs = [&](const NodePtr& current) {
            auto it = child_map.find(current);
            if (it != child_map.end()) {
                for (const NodePtr& child : it->second) {
                    child->cost_to_come = computeHeuristicCost(current, child->pos);
                    dfs(child);
                }
            }
        };

        dfs(node);
    }

    void grow(const int num_samples, const Vector2 goal, const Obstacles& obstacles, const bool rewire_enabled) {
        for (int i = 0; i < num_samples; ++i) {
            const Vector2 pos = sample(goal);
            growOnce(pos, obstacles, rewire_enabled);
        }
    }
};
