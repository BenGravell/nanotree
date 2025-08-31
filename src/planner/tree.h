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

NodePtr getCheapest(const Vector2 target, const Nodes& nodes, const float max_dist) {
    Nodes neighbors;
    neighbors.reserve(nodes.size());
    for (const NodePtr& node : nodes) {
        if (Vector2Distance(node->pos, target) <= max_dist) {
            neighbors.push_back(node);
        }
    }

    if (!neighbors.empty()) {
        return *std::min_element(neighbors.begin(), neighbors.end(), TargetCostComparator{target});
    }

    return getNearest(target, nodes);
}

Path extractPath(const Vector2 target, const Nodes& nodes) {
    Path path;
    NodePtr node = getCheapest(target, nodes, GOAL_RADIUS);
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
    static std::uniform_real_distribution<float> dist_goal_t(0.0f, TWO_PI);
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

struct Tree {
    Nodes nodes;

    void reset(const Vector2 start) {
        nodes = {std::make_shared<Node>(Node{nullptr, start, 0.0f})};
    }

    void carryover(const Path path, const int num_carryover, const bool carryover_path) {
        // TODO do not retain nodes or their children if in collision!

        Nodes retained_nodes;
        NodeSet retained_set;

        if (!path.empty() && carryover_path) {
            for (const NodePtr& node : path) {
                retained_nodes.push_back(node);
                retained_set.insert(node);
            }
        }

        NodePtr root = nodes.front();
        if (retained_set.find(root) == retained_set.end()) {
            retained_nodes.push_back(root);
            retained_set.insert(root);
        }

        Nodes shuffled = nodes;
        std::shuffle(shuffled.begin(), shuffled.end(), rng);
        for (const NodePtr& node : shuffled) {
            if (retained_nodes.size() > num_carryover) {
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
        for (int i = 0; i < num_samples; ++i) {
            Vector2 pos = ((i + 1) == num_samples) ? goal : sample(goal);
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

            nodes.push_back(std::make_shared<Node>(Node{parent, pos, computeHeuristicCost(parent, pos)}));
        }
    }
};
