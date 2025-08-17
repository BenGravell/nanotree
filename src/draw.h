#pragma once

#include <raylib.h>
#include <raymath.h>

#include "colors.h"
#include "config.h"
#include "mode.h"
#include "obstacle.h"
#include "tree.h"

float normalizeCost(const float c, const float c_goal, const float c_max) {
    float x = 0.0f;
    if (c < c_goal) {
        x = Remap(c / c_goal, 0.0f, 1.0f, 0.0f, 0.5f);
    } else {
        x = Remap((c - c_goal) / (c_max - c_goal), 0.0f, 1.0f, 0.5f, 1.0f);
    }
    return std::clamp(x, 0.0f, 1.0f);
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
        case SelectorMode::PLACE_GOAL:
            DrawSelector(pos, {GOAL_REACHED_RADIUS, 0.4f, 8, 4.0f});
            return;
        case SelectorMode::ADD_OBSTACLE:
            DrawSelector(pos, {OBSTACLE_RADIUS, 0.2f, 12, 5.0f});
            return;
        case SelectorMode::DEL_OBSTACLE:
            DrawSelector(pos, {OBSTACLE_DEL_RADIUS, 0.6f, 6, 3.0f});
            return;
    }
}

Color computeGoalColor(const bool goal_reached) {
    return goal_reached ? COLOR_GOAL_REACHED : COLOR_GOAL_NOT_REACHED;
}

void DrawGoal(const Vector2 goal, const bool goal_reached) {
    DrawCircleV(goal, GOAL_REACHED_RADIUS, Fade(computeGoalColor(goal_reached), 0.8f));
}

void DrawObstacles(const Obstacles& obstacles) {
    for (const Vector2 obstacle : obstacles) {
        DrawCircleV(obstacle, OBSTACLE_RADIUS, COLOR_OBSTACLE);
    }
}

void DrawTree(const Tree& tree, const Path& path) {
    float cost_to_come_goal = 0.0f;
    for (const auto& node : path) {
        cost_to_come_goal = std::max(cost_to_come_goal, node->cost_to_come);
    }
    float cost_to_come_max = 0.0f;
    for (const auto& node : tree.nodes) {
        cost_to_come_max = std::max(cost_to_come_max, node->cost_to_come);
    }
    for (const auto& node : tree.nodes) {
        if (!node->parent) {
            continue;
        }
        const Color color = computeCostColor(normalizeCost(node->cost_to_come, cost_to_come_goal, cost_to_come_max));
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_TREE, color);
    }
}

void DrawPath(const Path& path) {
    for (const auto& node : path) {
        if (!node->parent) {
            continue;
        }
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_PATH, COLOR_PATH);
        DrawCircleV(node->pos, 0.5f * NODE_WIDTH_PATH, COLOR_PATH);
    }
}

void DrawRibbon(const Tree& tree, const int num_samples, const int num_carryover, const bool goal_reached, const SelectorMode mode, const int fps, const std::vector<Rectangle>& button_rectangles, const Font font) {
    DrawRectangle(0, ENVIRONMENT_HEIGHT, ENVIRONMENT_WIDTH, RIBBON_HEIGHT, COLOR_RIBBON_BACKGROUND);

    for (const auto rect : button_rectangles) {
        DrawRectangleLinesEx(rect, 3, COLOR_TEXT_CONTROL_SELECT_BKGD);
    }

    // TODO use the Rectangle objects directly
    // TODO make class for clickable button to handle color of button and text

    // highlight the selected mode cell
    for (int m = 0; m <= 2; ++m) {
        if (m == int(mode)) {
            DrawRectangleRec(button_rectangles[m], COLOR_TEXT_CONTROL_SELECT_BKGD);
        }
    }

    static constexpr int TEXT_MARGIN_WIDTH = 20;

    static constexpr int RIBBON_ROW_1A_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 15;
    static constexpr int RIBBON_ROW_1B_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 60;
    static constexpr int RIBBON_ROW_2_Y = ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;

    // TODO center all text

    // row 2
    DrawTextEx(font, std::string(goal_reached ? "Goal reached" : "Goal missed").c_str(), {0.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, computeGoalColor(goal_reached));

    DrawTextEx(font, TextFormat("%2i FPS", fps), {1.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, computeFpsColor(fps));

    DrawTextEx(font, (std::to_string(tree.nodes.size()) + " nodes").c_str(), {1.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_NODE_COUNT);

    DrawTextEx(font, (std::to_string(num_carryover) + " carryover").c_str(), {2.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_NODE_COUNT);

    DrawTextEx(font, (std::to_string(num_samples) + " samples").c_str(), {3.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y}, TEXT_HEIGHT_STAT, 1, COLOR_NODE_COUNT);

    // row 1a
    DrawTextEx(font, "Place goal", {0.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::PLACE_GOAL) ? COLOR_RIBBON_BACKGROUND : COLOR_TEXT_CONTROLS);
    DrawTextEx(font, "- obstacle", {1.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::DEL_OBSTACLE) ? COLOR_RIBBON_BACKGROUND : COLOR_TEXT_CONTROLS);
    DrawTextEx(font, "+ obstacle", {1.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, (mode == SelectorMode::ADD_OBSTACLE) ? COLOR_RIBBON_BACKGROUND : COLOR_TEXT_CONTROLS);

    DrawTextEx(font, "- carryover", {2.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, COLOR_TEXT_CONTROLS);
    DrawTextEx(font, "+ carryover", {2.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, COLOR_TEXT_CONTROLS);

    DrawTextEx(font, "- samples", {3.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, COLOR_TEXT_CONTROLS);
    DrawTextEx(font, "+ samples", {3.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y}, TEXT_HEIGHT_CONTROL_MODE, 1, COLOR_TEXT_CONTROLS);

    // row 1b
    DrawTextEx(font, "[LMB] to engage", {0.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y}, TEXT_HEIGHT_CONTROL_KEYMAP, 1, (mode == SelectorMode::PLACE_GOAL) ? COLOR_RIBBON_BACKGROUND : COLOR_TEXT_CONTROLS);

    DrawTextEx(font, "[LMB] here / [MMB]", {1.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y}, TEXT_HEIGHT_CONTROL_KEYMAP, 1, (mode == SelectorMode::DEL_OBSTACLE) ? COLOR_RIBBON_BACKGROUND : COLOR_TEXT_CONTROLS);
    DrawTextEx(font, "[LMB] here / [RMB]", {1.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y}, TEXT_HEIGHT_CONTROL_KEYMAP, 1, (mode == SelectorMode::ADD_OBSTACLE) ? COLOR_RIBBON_BACKGROUND : COLOR_TEXT_CONTROLS);

    DrawTextEx(font, "[LMB] here", {2.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y}, TEXT_HEIGHT_CONTROL_KEYMAP, 1, COLOR_TEXT_CONTROLS);
    DrawTextEx(font, "[LMB] here", {2.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y}, TEXT_HEIGHT_CONTROL_KEYMAP, 1, COLOR_TEXT_CONTROLS);

    DrawTextEx(font, "[LMB] here / [Scroll]", {3.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y}, TEXT_HEIGHT_CONTROL_KEYMAP, 1, COLOR_TEXT_CONTROLS);
    DrawTextEx(font, "[LMB] here / [Scroll]", {3.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y}, TEXT_HEIGHT_CONTROL_KEYMAP, 1, COLOR_TEXT_CONTROLS);
}