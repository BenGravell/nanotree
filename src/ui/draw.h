#pragma once

#include <raylib.h>
#include <raymath.h>

#include "core/config.h"
#include "core/obstacle.h"
#include "planner/tree.h"
#include "ui/colors.h"
#include "ui/mode.h"
#include "ui/number_widget.h"

void DrawChevron(const Rectangle& rec, const float dir, const Color color) {
    const Vector2 c = {rec.x + 0.5f * rec.width, rec.y + 0.5f * rec.height};
    const float s = 0.5f * 0.75f * std::min(rec.width, rec.height);

    Vector2 v[6];
    const float x_scale = std::abs(dir);

    // TODO make this less verbose
    if (dir > 0.0f) {
        // Right-facing outline (top-left -> mid-top -> tip -> mid-bot -> bot-left -> inner-notch)
        v[0] = {c.x - x_scale * s, c.y - s};  // top-left
        v[1] = {c.x, c.y - s};                // mid-top
        v[2] = {c.x + x_scale * s, c.y};      // tip (right)
        v[3] = {c.x, c.y + s};                // mid-bot
        v[4] = {c.x - x_scale * s, c.y + s};  // bot-left
        v[5] = {c.x - x_scale, c.y};          // inner notch (left of center)

        DrawTriangle(v[5], v[1], v[0], color);
        DrawTriangle(v[5], v[2], v[1], color);
        DrawTriangle(v[5], v[3], v[2], color);
        DrawTriangle(v[5], v[4], v[3], color);
    } else {
        // Left-facing outline (top-right -> mid-top -> tip -> mid-bot -> bot-right -> inner-notch)
        v[0] = {c.x + x_scale * s, c.y - s};  // top-right
        v[1] = {c.x, c.y - s};                // mid-top
        v[2] = {c.x - x_scale * s, c.y};      // tip (left)
        v[3] = {c.x, c.y + s};                // mid-bot
        v[4] = {c.x + x_scale * s, c.y + s};  // bot-right
        v[5] = {c.x + x_scale, c.y};          // inner notch (right of center)

        DrawTriangle(v[0], v[1], v[5], color);
        DrawTriangle(v[1], v[2], v[5], color);
        DrawTriangle(v[2], v[3], v[5], color);
        DrawTriangle(v[3], v[4], v[5], color);
    }
}

void DrawDoubleChevron(const Rectangle& rec, const float dir, const Color color) {
    const float offset = 0.18f * rec.width;
    const float mult = (dir > 0.0f) ? 1.0f : -1.0f;

    Rectangle rec1 = rec;
    Rectangle rec2 = rec;

    rec1.x += mult * offset;
    rec2.x -= mult * offset;

    DrawChevron(rec1, dir, color);
    DrawChevron(rec2, dir, color);
}

void DrawScrollWheel(Rectangle bounds, Color color, const float thickness) {
    const float center_x = bounds.x + 0.5f * bounds.width;
    const float center_y = bounds.y + 0.5f * bounds.height;

    // Dimensions relative to rectangle
    const float body_height = 0.7f * bounds.height;
    const float body_width = 0.3f * bounds.width;

    // Draw scroll wheel body (oval/rounded rectangle)
    Rectangle body = {
        center_x - 0.5f * body_width,
        center_y - 0.5f * body_height,
        body_width,
        body_height};
    DrawRectangleRoundedLinesEx(body, 1.0f, 16, thickness, color);

    // Draw horizontal bars inside wheel
    const int lines = 5;
    const float bar_width_min = 0.3f * body_width;
    const float bar_width_max = 0.7f * body_width;
    for (int i = 0; i < lines; i++) {
        const float t = (i + 1.0f) / (lines + 1.0f);
        const float y = body.y + t * body_height;
        const float bar_width = Lerp(bar_width_min, bar_width_max, (1.0f - 2.0f * std::abs(t - 0.5)));
        DrawLineEx({center_x - 0.5f * bar_width, y},
                   {center_x + 0.5f * bar_width, y},
                   thickness,
                   color);
    }
}

void DrawToggleButton(const ToggleButton& b) {
    DrawRectangleRec(b.rec, b.engaged ? COLOR_BUTTON_BACKGROUND_ACTIVE : COLOR_BUTTON_BACKGROUND_INACTIVE);
    DrawRectangleLinesEx(b.rec, 3.0f, b.engaged ? COLOR_BUTTON_BORDER_ACTIVE : COLOR_BUTTON_BORDER_INACTIVE);
    DrawScrollWheel(b.rec, b.engaged ? COLOR_BUTTON_CONTENT_ACTIVE : COLOR_BUTTON_CONTENT_INACTIVE, b.engaged ? 5.0f : 3.0f);
}

void DrawDeltaButtonGroup(const DeltaButtonGroup& b) {
    for (const auto& [delta, rec] : b.rectangles) {
        DrawRectangleRec(rec, COLOR_BUTTON_BACKGROUND_INACTIVE);
        DrawRectangleLinesEx(rec, 3.0f, COLOR_BUTTON_BORDER_INACTIVE);
        const int delta_int = static_cast<int>(delta);
        const int delta_sign = delta_int > 0 ? 1 : -1;
        const float dir = 0.6f * delta_sign;
        if (abs(delta_int) > 1) {
            DrawDoubleChevron(rec, dir, COLOR_BUTTON_CONTENT_INACTIVE);
        } else {
            DrawChevron(rec, dir, COLOR_BUTTON_CONTENT_INACTIVE);
        }
    }
}

void DrawNumberWidget(const NumberWidget& w) {
    DrawDeltaButtonGroup(w.delta_button_group);
    DrawToggleButton(w.scroll_toggle_button);
}

struct SelectorParams {
    float radius;
    float ring_width_frac;
    int num_segments;
    float period_sec;
};

void DrawSelector(const Vector2 pos, const SelectorParams params) {
    const float delta_angle = 360.0f / (2.0f * params.num_segments);
    const float ring_outer_radius = params.radius;
    const float ring_width = params.ring_width_frac * params.radius;
    const float ring_inner_radius = ring_outer_radius - ring_width;
    const float current_time = GetTime();
    const float offset_angle = fmodf(current_time / params.period_sec, 1.0f) * 360.0f;
    DrawCircleV(pos, params.radius, Fade(GRAY, 0.4f));
    DrawRing(pos, ring_outer_radius - ring_width, ring_outer_radius, 0, 360, 0, Fade(GRAY, 0.6f));
    for (int i = 0; i < params.num_segments; ++i) {
        const float start_angle = 2 * i * delta_angle + offset_angle;
        DrawRing(pos, ring_inner_radius, ring_outer_radius, start_angle, start_angle + delta_angle, 0, LIGHTGRAY);
    }
}

void DrawSelectorByMode(const Vector2 pos, const SelectorMode mode) {
    switch (mode) {
        case SelectorMode::PLACE_START:
            DrawSelector(pos, {START_RADIUS, 0.6f, 6, 3.0f});
            return;
        case SelectorMode::PLACE_GOAL:
            DrawSelector(pos, {GOAL_RADIUS, 0.4f, 8, 4.0f});
            return;
        case SelectorMode::ADD_OBSTACLE:
            DrawSelector(pos, {OBSTACLE_RADIUS, 0.2f, 12, 5.0f});
            return;
        case SelectorMode::DEL_OBSTACLE:
            DrawSelector(pos, {OBSTACLE_DEL_RADIUS, 0.6f, 6, 3.0f});
            return;
    }
}

struct FlatGridParams {
    int spacing;
    int thickness;
    Color color;
};

void DrawFlatGrid(const int x_min, const int x_max, const int y_min, const int y_max, const FlatGridParams params) {
    for (int x = x_min; x <= x_max; x += params.spacing) {
        DrawLineEx({float(x), float(y_min)}, {float(x), float(y_max)}, params.thickness, params.color);
    }
    for (int y = y_min; y <= y_max; y += params.spacing) {
        DrawLineEx({float(x_min), float(y)}, {float(x_max), float(y)}, params.thickness, params.color);
    }
}

void DrawSquare(const Vector2 center, const float radius, const Color color) {
    DrawRectangle(center.x - radius, center.y - radius, 2 * radius, 2 * radius, color);
}

Color computeGoalColor(const bool goal_reached) {
    return goal_reached ? COLOR_GOAL_REACHED : COLOR_GOAL_NOT_REACHED;
}

void DrawStart(const Vector2 start) {
    DrawCircleV(start, START_RADIUS, Fade(COLOR_START, 0.8f));
}

void DrawGoal(const Vector2 goal, const bool goal_reached) {
    DrawCircleV(goal, GOAL_RADIUS, Fade(computeGoalColor(goal_reached), 0.8f));
}

void DrawObstacles(const Obstacles& obstacles) {
    for (const Vector2 obstacle : obstacles) {
        DrawCircleV(obstacle, OBSTACLE_RADIUS, COLOR_OBSTACLE);
    }
}

float normalizeCost(const float c, const float c_root, const float c_path, const float c_tree) {
    float x = 0.0f;
    if (c < c_path) {
        x = Remap((c - c_root) / (c_path - c_root), 0.0f, 1.0f, 0.0f, 0.5f);
    } else {
        x = Remap((c - c_path) / (c_tree - c_path), 0.0f, 1.0f, 0.5f, 1.0f);
    }
    return std::clamp(x, 0.0f, 1.0f);
}

void DrawTree(const Tree& tree, const Path& path, const Vector2 goal) {
    float cost_root = computeHeuristicCost(tree.nodes.front(), goal);

    float cost_path = 0.0f;
    for (const NodePtr& node : path) {
        cost_path = std::max(cost_path, computeHeuristicCost(node, goal));
    }

    float cost_tree = 0.0f;
    for (const NodePtr& node : tree.nodes) {
        cost_tree = std::max(cost_tree, computeHeuristicCost(node, goal));
    }

    // Sort by heuristic cost
    std::vector<NodePtr> sorted_nodes = tree.nodes;
    std::sort(sorted_nodes.begin(), sorted_nodes.end(), TargetCostComparatorInv{goal});

    // Draw in sorted order
    for (const NodePtr& node : sorted_nodes) {
        if (!node->parent) {
            continue;
        }
        const Color color = computeCostColor(normalizeCost(computeHeuristicCost(node, goal), cost_root, cost_path, cost_tree));
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_TREE, color);
    }
}

void DrawPath(const Path& path) {
    for (const NodePtr& node : path) {
        if (!node->parent) {
            continue;
        }
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_PATH, COLOR_PATH);
        DrawCircleV(node->pos, 0.5f * NODE_WIDTH_PATH, COLOR_PATH);
    }
}

void DrawRibbon(const Tree& tree, const int num_samples, const int num_carryover, const bool goal_reached, const SelectorMode mode, const int fps, const std::vector<Rectangle>& button_rectangles, const Font font) {
    DrawRectangle(0, ENVIRONMENT_HEIGHT, ENVIRONMENT_WIDTH, RIBBON_HEIGHT, COLOR_RIBBON_BACKGROUND);

    for (const Rectangle rect : button_rectangles) {
        DrawRectangleLinesEx(rect, 3, COLOR_TEXT_CONTROL_SELECT_BKGD);
    }

    for (int i = 1; i < 4; ++i) {
        DrawLineEx({float(i) * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 10}, {float(i) * RIBBON_COL_WIDTH, ENVIRONMENT_HEIGHT + 2 * RIBBON_ROW_HEIGHT - 10}, 3, Color{128, 128, 128, 255});
    }

    // TODO use the Rectangle objects directly
    // TODO make class for clickable button to handle color of button and text

    // TODO access rectangle by enum mode explicitly instead of relying on integer cast and index alignment
    // highlight the selected mode cell
    for (int m = 0; m <= 3; ++m) {
        if (m == int(mode)) {
            DrawRectangleRec(button_rectangles[m], COLOR_TEXT_CONTROL_SELECT_BKGD);
        }
    }

    static constexpr int TEXT_MARGIN_WIDTH = 20;

    static constexpr int RIBBON_ROW_1_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_CONTROL_MODE) / 2;
    static constexpr int RIBBON_ROW_2_Y = ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;

    // TODO center all text

    // row 2
    DrawTextEx(font, std::string(goal_reached ? "Goal reached" : "Goal missed").c_str(), {0.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, computeGoalColor(goal_reached));

    DrawTextEx(font, TextFormat("%2i FPS", fps), {1.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, computeFpsColor(fps));

    DrawTextEx(font, (std::to_string(tree.nodes.size()) + " nodes").c_str(), {1.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, 0.6 * TEXT_HEIGHT_STAT, 1, COLOR_NODE_COUNT);

    DrawTextEx(font, ("Samples = " + std::to_string(num_samples)).c_str(), {2.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_NODE_COUNT);
    DrawTextEx(font, ("Carryover = " + std::to_string(num_carryover)).c_str(), {3.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_NODE_COUNT);

    // row 1a
    DrawTextEx(font, "Place start", {0.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::PLACE_START) ? COLOR_BUTTON_BACKGROUND_INACTIVE : COLOR_BUTTON_BACKGROUND_ACTIVE);
    DrawTextEx(font, "Place goal", {0.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::PLACE_GOAL) ? COLOR_BUTTON_BACKGROUND_INACTIVE : COLOR_BUTTON_BACKGROUND_ACTIVE);
    DrawTextEx(font, "- Obstacle", {1.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::DEL_OBSTACLE) ? COLOR_BUTTON_BACKGROUND_INACTIVE : COLOR_BUTTON_BACKGROUND_ACTIVE);
    DrawTextEx(font, "+ Obstacle", {1.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::ADD_OBSTACLE) ? COLOR_BUTTON_BACKGROUND_INACTIVE : COLOR_BUTTON_BACKGROUND_ACTIVE);
}