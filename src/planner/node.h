#pragma once

#include <raylib.h>

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include "cost.h"

struct Node;

using NodePtr = std::shared_ptr<Node>;
using Nodes = std::vector<NodePtr>;

struct Node {
    NodePtr parent;
    Vector2 pos;
    float cost_to_come;

    float estimateCostTo(const Vector2 goal) {
        return cost_to_come + computeCost(pos, goal);
    }
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

using NodeSet = std::unordered_set<NodePtr, NodePtrHash, NodePtrEq>;