#pragma once

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <random>
#include <unordered_set>
#include <vector>

#include "geometry.h"
#include "obstacle.h"
#include "rng.h"

struct Node;

using NodePtr = std::shared_ptr<Node>;
using Nodes = std::vector<NodePtr>;
using Path = std::vector<NodePtr>;

struct Node {
    NodePtr parent;
    Vector2 pos;
    float cost_to_come;
};

struct NodePtrHash {
    std::size_t operator()(const NodePtr& n) const noexcept {
        return std::hash<Node*>()(n.get());
    }
};

struct NodePtrEq {
    bool operator()(const NodePtr& a, const NodePtr& b) const noexcept {
        return a.get() == b.get();
    }
};

struct TargetDistanceComparator {
    Vector2 target;

    bool operator()(const NodePtr& a, const NodePtr& b) const {
        return Vector2Distance(a->pos, target) < Vector2Distance(b->pos, target);
    }
};

struct TargetCostComparator {
    Vector2 target;

    bool operator()(const NodePtr& a, const NodePtr& b) const {
        return (a->cost_to_come + Vector2Distance(a->pos, target)) < (b->cost_to_come + Vector2Distance(b->pos, target));
    }
};

NodePtr getNearest(const Vector2 target, const Nodes& nodes) {
    return *std::min_element(nodes.begin(), nodes.end(), TargetDistanceComparator{target});
}

NodePtr getCheapest(const Vector2 target, const Nodes& nodes, const float max_dist) {
    std::vector<NodePtr> within_range;
    within_range.reserve(nodes.size());
    for (const NodePtr& node : nodes) {
        if (Vector2Distance(node->pos, target) <= max_dist) {
            within_range.push_back(node);
        }
    }

    if (!within_range.empty()) {
        return *std::min_element(within_range.begin(), within_range.end(), TargetCostComparator{target});
    }

    return getNearest(target, nodes);
}

Path extractPath(const Vector2 target, const Nodes& nodes) {
    Path path;
    NodePtr node = getCheapest(target, nodes, GOAL_REACHED_RADIUS);
    while (node->parent) {
        path.push_back(node);
        node = node->parent;
    }
    path.push_back(node);
    std::reverse(path.begin(), path.end());
    return path;
}

Vector2 sample(const Vector2 goal) {
    static std::uniform_real_distribution<float> dist_select(0.0f, 1.0f);
    static std::uniform_real_distribution<float> dist_x(0.0f, ENVIRONMENT_WIDTH);
    static std::uniform_real_distribution<float> dist_y(0.0f, ENVIRONMENT_HEIGHT);
    static std::uniform_real_distribution<float> dist_goal_x(-GOAL_REACHED_RADIUS, GOAL_REACHED_RADIUS);
    static std::uniform_real_distribution<float> dist_goal_y(-GOAL_REACHED_RADIUS, GOAL_REACHED_RADIUS);

    if (dist_select(rng) < GOAL_SAMPLE_PROBABILITY) {
        return clampToEnvironment(goal + Vector2{dist_goal_x(rng), dist_goal_y(rng)});
    }

    return Vector2{dist_x(rng), dist_y(rng)};
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

struct Tree {
    Nodes nodes;

    void reset() {
        nodes = {std::make_shared<Node>(Node{nullptr, DEFAULT_START, 0.0f})};
    }

    void retain(const Path path, const int target_max_size) {
        // TODO do not retain nodes or their children if in collision!

        std::vector<NodePtr> retained_nodes;
        std::unordered_set<NodePtr, NodePtrHash, NodePtrEq> retained_set;

        if (!path.empty()) {
            for (const NodePtr& node : nodes) {
                if (std::find(path.begin(), path.end(), node) != path.end()) {
                    retained_nodes.push_back(node);
                    retained_set.insert(node);
                }
            }
        }

        Nodes shuffled = nodes;
        std::shuffle(shuffled.begin(), shuffled.end(), rng);
        for (const NodePtr& node : shuffled) {
            if (retained_nodes.size() > target_max_size) {
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

        if (!retained_nodes.empty()) {
            nodes = std::move(retained_nodes);
        }
    }

    void grow(const int num_samples, const Vector2 goal, const Obstacles& obstacles) {
        for (int i = 0; i <= num_samples; ++i) {
            Vector2 pos = (i == num_samples) ? goal : sample(goal);
            NodePtr parent = getCheapest(pos, nodes, CHEAP_PARENT_SEARCH_RADIUS);
            pos = clampToEnvironment(pos);
            pos = attractByDistance(pos, parent);
            pos = attractByAngle(pos, parent);

            if (!insideEnvironment(pos)) {
                continue;
            }

            if (collides(pos, obstacles)) {
                continue;
            }

            const float cost = Vector2Distance(parent->pos, pos);
            nodes.push_back(std::make_shared<Node>(Node{parent, pos, parent->cost_to_come + cost}));
        }
    }
};
