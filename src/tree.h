#pragma once

#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include "geometry.h"
#include "obstacle.h"
#include "rng.h"

struct Node {
    std::shared_ptr<Node> parent;
    Vector2 pos;
    float cost_to_come;
};

using Nodes = std::vector<std::shared_ptr<Node>>;
using Path = std::vector<std::shared_ptr<Node>>;

struct TargetDistanceComparator {
    Vector2 target;

    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
        return Vector2Distance(a->pos, target) < Vector2Distance(b->pos, target);
    }
};

struct TargetCostComparator {
    Vector2 target;

    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
        return (a->cost_to_come + Vector2Distance(a->pos, target)) < (b->cost_to_come + Vector2Distance(b->pos, target));
    }
};

std::shared_ptr<Node> getNearest(const Vector2 target, const Nodes& nodes) {
    return *std::min_element(nodes.begin(), nodes.end(), TargetDistanceComparator{target});
}

std::shared_ptr<Node> getCheapest(const Vector2 target, const Nodes& nodes, const float max_dist) {
    std::vector<std::shared_ptr<Node>> within_range;
    within_range.reserve(nodes.size());
    for (const auto& node : nodes) {
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
    std::shared_ptr<Node> node = getCheapest(target, nodes, GOAL_REACHED_RADIUS);
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

Vector2 attractByDistance(const Vector2 pos, const std::shared_ptr<Node> parent) {
    const Vector2 direction = Vector2Normalize(pos - parent->pos);
    const float distance = std::min(DEVIATION_DISTANCE_MAX, Vector2Distance(parent->pos, pos));
    return Vector2Add(parent->pos, direction * distance);
}

Vector2 attractByAngle(const Vector2 pos, const std::shared_ptr<Node> parent) {
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

    void grow(const int num_samples, const Vector2 goal, const Obstacles& obstacles) {
        for (int i = 0; i <= num_samples; ++i) {
            Vector2 pos = (i == num_samples) ? goal : sample(goal);
            std::shared_ptr<Node> parent = getCheapest(pos, nodes, CHEAP_PARENT_SEARCH_RADIUS);
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
