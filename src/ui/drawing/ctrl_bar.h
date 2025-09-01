#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"
#include "ui/ctrl_state.h"
#include "ui/label.h"

static constexpr int CTRL_BAR_ROW_0_Y = CTRL_BAR_Y_MIN + 0 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_1_Y = CTRL_BAR_Y_MIN + 1 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_2_Y = CTRL_BAR_Y_MIN + 2 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_3_Y = CTRL_BAR_Y_MIN + 3 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_4_Y = CTRL_BAR_Y_MIN + 4 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_5_Y = CTRL_BAR_Y_MIN + 5 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_6_Y = CTRL_BAR_Y_MIN + 6 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_7_Y = CTRL_BAR_Y_MIN + 7 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_8_Y = CTRL_BAR_Y_MIN + 8 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_9_Y = CTRL_BAR_Y_MIN + 9 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_10_Y = CTRL_BAR_Y_MIN + 10 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_11_Y = CTRL_BAR_Y_MIN + 11 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_12_Y = CTRL_BAR_Y_MIN + 12 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_13_Y = CTRL_BAR_Y_MIN + 13 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_14_Y = CTRL_BAR_Y_MIN + 14 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_15_Y = CTRL_BAR_Y_MIN + 15 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_16_Y = CTRL_BAR_Y_MIN + 16 * CTRL_BAR_ROW_HEIGHT;
static constexpr int CTRL_BAR_ROW_17_Y = CTRL_BAR_Y_MIN + 17 * CTRL_BAR_ROW_HEIGHT;

static constexpr int CTRL_BAR_COL_WIDTH = CTRL_BAR_WIDTH / 2;
static constexpr int CTRL_BAR_COL_0_X = CTRL_BAR_X_MIN + 0 * CTRL_BAR_COL_WIDTH;
static constexpr int CTRL_BAR_COL_1_X = CTRL_BAR_X_MIN + 1 * CTRL_BAR_COL_WIDTH;

static constexpr int CTRL_BAR_BUTTON_HEIGHT = 2 * CTRL_BAR_ROW_HEIGHT - BUTTON_SPACING_Y;
static constexpr int CTRL_BAR_BUTTON_WIDTH = CTRL_BAR_COL_WIDTH - 2 * BUTTON_SPACING_X;
static constexpr int CTRL_BAR_BUTTON_X_MIN = CTRL_BAR_X_MIN + BUTTON_SPACING_X;
static constexpr int CTRL_BAR_BUTTON_X_MAX = CTRL_BAR_BUTTON_X_MIN + CTRL_BAR_BUTTON_WIDTH;
static constexpr int CTRL_BAR_WIDE_BUTTON_WIDTH = CTRL_BAR_WIDTH - 2 * BUTTON_SPACING_X;

template <size_t N>
void GuiLabelSpinner(Rectangle bounds, const char* label_text, int* ix, std::array<int, N> options) {
    GuiSpinner(bounds, NULL, ix, 0, N - 1, false);
    const Rectangle label_bounds = {bounds.x, bounds.y + bounds.height + BUTTON_SPACING_Y / 2, bounds.width, TEXT_HEIGHT};
    GuiLabelValue(label_bounds, label_text, TextFormat("%6d", options[*ix]));
}

CtrlState DrawCtrlBar(CtrlState state, const bool trigger_tree_reset, const bool goal_reached) {
    // Background
    DrawRectangleRec(CTRL_BAR_REC, COLOR_STAT_BAR_BACKGROUND);

    int selector_mode_int = static_cast<int>(state.selector_mode);
    const Rectangle selector_mode_bounds = {CTRL_BAR_COL_0_X + BUTTON_SPACING_X, CTRL_BAR_ROW_1_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT};

    GuiToggleGroup(selector_mode_bounds, "Place Goal\nPlace Start\nAdd Obstacle\nRemove Obstacle", &selector_mode_int);
    state.selector_mode = static_cast<SelectorMode>(selector_mode_int);

    GuiToggle((Rectangle){CTRL_BAR_COL_0_X + BUTTON_SPACING_X, CTRL_BAR_ROW_9_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Snap\nTo Grid", &state.snap_to_grid);

    // Vertical line separator
    DrawLineEx({CTRL_BAR_COL_1_X, CTRL_BAR_ROW_1_Y + BUTTON_SPACING_Y}, {CTRL_BAR_COL_1_X, CTRL_BAR_ROW_11_Y}, BORDER_THICKNESS, COLOR_GRAY_064);
    // Horizontal line separator
    DrawLineEx({CTRL_BAR_COL_0_X + 2 * BUTTON_SPACING_X, CTRL_BAR_ROW_11_Y}, {CTRL_BAR_COL_1_X, CTRL_BAR_ROW_11_Y}, BORDER_THICKNESS, COLOR_GRAY_064);

    int tree_growth_mode_int = static_cast<int>(state.tree_growth_mode);
    const Rectangle tree_growth_mode_bounds = {CTRL_BAR_COL_1_X + BUTTON_SPACING_X, CTRL_BAR_ROW_1_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT};
    GuiToggleGroup(tree_growth_mode_bounds, "Play\nUntil Goal\nPause", &tree_growth_mode_int);
    state.tree_growth_mode = static_cast<TreeGrowthMode>(tree_growth_mode_int);

    const bool tree_should_grow_once = GuiButton((Rectangle){CTRL_BAR_COL_1_X + BUTTON_SPACING_X, CTRL_BAR_ROW_7_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Advance");
    state.tree_should_reset = GuiButton((Rectangle){CTRL_BAR_COL_1_X + BUTTON_SPACING_X, CTRL_BAR_ROW_9_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Reset");
    if (trigger_tree_reset) {
        state.tree_should_reset = true;
    }

    state.tree_should_grow = (state.tree_growth_mode == TreeGrowthMode::ALWAYS) || ((state.tree_growth_mode == TreeGrowthMode::UNTIL_GOAL_REACHED) && !goal_reached) || tree_should_grow_once;

    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_WIDTH, CTRL_BAR_BUTTON_WIDTH);
    GuiLabelSpinner((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_11_Y + BUTTON_SPACING_Y, CTRL_BAR_WIDE_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Carryover", &state.num_carryover_ix, NUM_CARRYOVER_OPTIONS);
    GuiLabelSpinner((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_14_Y, CTRL_BAR_WIDE_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Samples", &state.num_samples_ix, NUM_SAMPLES_OPTIONS);

    // Border
    DrawRectangleLinesEx(CTRL_BAR_REC, 3, COLOR_STAT_BAR_BORDER);

    return state;
}