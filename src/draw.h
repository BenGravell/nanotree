#pragma once

#include <raylib.h>
#include <raymath.h>

#include "config.h"
#include "obstacle.h"
#include "pride.h"
#include "tree.h"

Color fpsColor(const int fps) {
    if (fps < 15) return COLOR_FPS_LOW;
    if (fps < 30) return COLOR_FPS_MID;
    return COLOR_FPS_HIGH;
}

Color mapToColor(float x) {
    x = Remap(x, 0.0f, 1.0f, 0.2f, 0.8f);
    const int idx = std::clamp(static_cast<int>(x * 255.0f + 0.5f), 0, 255);
    const auto& rgb = pride_colormap[idx];
    return Color{rgb[0], rgb[1], rgb[2], 255};
}

float normalizeCost(const float c, const float c_goal, const float c_max) {
    float x = 0.0f;
    if (c < c_goal) {
        x = Remap(c / c_goal, 0.0f, 1.0f, 0.0f, 0.5f);
    } else {
        x = Remap((c - c_goal) / (c_max - c_goal), 0.0f, 1.0f, 0.5f, 1.0f);
    }
    return std::clamp(x, 0.0f, 1.0f);
}

void DrawSelector(const Vector2 pos, const float radius, const float ring_width_frac, const int num_segments, const float selector_orbit_period_sec) {
    const float delta_angle = 360.0f / (2.0f * num_segments);
    const float ring_outer_radius = radius;
    const float ring_width = ring_width_frac * radius;
    const float ring_inner_radius = ring_outer_radius - ring_width;
    const float current_time = GetTime();
    const float offset_angle = fmodf(current_time / selector_orbit_period_sec, 1.0f) * 360.0f;
    DrawCircleV(pos, radius, Fade(GRAY, 0.4f));
    DrawRing(pos, ring_outer_radius - ring_width, ring_outer_radius, 0, 360, 0, Fade(GRAY, 0.6f));
    for (int i = 0; i < num_segments; ++i) {
        const float start_angle = 2 * i * delta_angle + offset_angle;
        DrawRing(pos, ring_inner_radius, ring_outer_radius, start_angle, start_angle + delta_angle, 0, LIGHTGRAY);
    }
}

void DrawSelectorByMode(const Vector2 pos, const int mode) {
    if (mode == 1) {
        DrawSelector(pos, GOAL_REACHED_RADIUS, 0.4f, 8, 4.0f);
    }
    if (mode == 2) {
        DrawSelector(pos, OBSTACLE_RADIUS, 0.2f, 12, 5.0f);
    }
    if (mode == 3) {
        DrawSelector(pos, OBSTACLE_DEL_RADIUS, 0.6f, 6, 3.0f);
    }
}

Color getGoalColor(const bool goal_reached) {
    return goal_reached ? COLOR_GOAL_REACHED : COLOR_GOAL_NOT_REACHED;
}

void DrawGoal(const Vector2 goal, const bool goal_reached) {
    DrawCircleV(goal, GOAL_REACHED_RADIUS, Fade(getGoalColor(goal_reached), 0.8f));
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
        const Color color = mapToColor(normalizeCost(node->cost_to_come, cost_to_come_goal, cost_to_come_max));
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_TREE, color);
    }
}

void DrawPath(const Path& path) {
    for (const auto& node : path) {
        if (!node->parent) {
            continue;
        }
        DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_PATH, COLOR_PATH);
        DrawCircleV(node->pos, NODE_WIDTH_PATH / 2, COLOR_PATH);
    }
}

void DrawRibbon(const Tree& tree, const int num_samples, const bool goal_reached, const int mode, const int fps, const std::vector<Rectangle>& button_rectangles) {
    DrawRectangle(0, ENVIRONMENT_HEIGHT, ENVIRONMENT_WIDTH, RIBBON_HEIGHT, COLOR_RIBBON_BACKGROUND);

    for (const auto rect : button_rectangles) {
            DrawRectangleLinesEx(rect, 3, COLOR_KEYMAP);

    }

    // TODO use the Rectangle objects directly
    // TODO make class for clickable button to handle color of button and text
    for (int m = 1; m <= 3; ++m) {
        if (m == mode) {
            int x = (m - 1) * RIBBON_COL_WIDTH;
            int y = ENVIRONMENT_HEIGHT;
            DrawRectangle(x, y, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT, COLOR_KEYMAP);
        }
    }

    static constexpr int TEXT_MARGIN_WIDTH = 20;

    static constexpr int RIBBON_ROW_1A_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 15;
    static constexpr int RIBBON_ROW_1B_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + 60;
    static constexpr int RIBBON_ROW_2_Y = ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT_STAT) / 2;

    // TODO center all text
    DrawText(std::string(goal_reached ? "Goal reached" : "Goal missed").c_str(), 0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT_STAT, getGoalColor(goal_reached));
    DrawText(TextFormat("%2i FPS", fps), 1 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT_STAT, fpsColor(fps));
    DrawText((std::to_string(tree.nodes.size()) + " nodes").c_str(), 2 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT_STAT, COLOR_NODE_COUNT);
    DrawText((std::to_string(num_samples) + " samples").c_str(), 3 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT_STAT, COLOR_NODE_COUNT);

    DrawText("Place goal", 0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, (mode == 1) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
    DrawText("Insert obstacle", 1 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, (mode == 2) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
    DrawText("Delete obstacle", 2 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, (mode == 3) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
    DrawText("- samples", 3.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, COLOR_KEYMAP);
    DrawText("+ samples", 3.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1A_Y, TEXT_HEIGHT_CONTROL_MODE, COLOR_KEYMAP);

    DrawText("[LMB] to engage", 0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, (mode == 1) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
    DrawText("[LMB] to engage / [RMB]", 1 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, (mode == 2) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
    DrawText("[LMB] to engage / [MMB]", 2 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, (mode == 3) ? COLOR_RIBBON_BACKGROUND : COLOR_KEYMAP);
    DrawText("[LMB] here / [Scroll]", 3.0 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, COLOR_KEYMAP);
    DrawText("[LMB] here / [Scroll]", 3.5 * RIBBON_COL_WIDTH + TEXT_MARGIN_WIDTH, RIBBON_ROW_1B_Y, TEXT_HEIGHT_CONTROL_KEYMAP, COLOR_KEYMAP);
}