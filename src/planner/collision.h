#pragma once

#include "core/problem/obstacle.h"
#include "planner/node.h"

bool edgeCollides(const NodePtr &node, const Obstacles &obstacles) {
  return (node->parent) ? collides(node->parent->pos, node->pos, obstacles) : collides(node->pos, obstacles);
}
