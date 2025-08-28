#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"
#include "ui/ctrl_state.h"

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

static constexpr int CTRL_BAR_BUTTON_WIDTH = CTRL_BAR_WIDTH - 2 * BUTTON_SPACING_X;
static constexpr int CTRL_BAR_BUTTON_X_MIN = CTRL_BAR_X_MIN + BUTTON_SPACING_X;
static constexpr int CTRL_BAR_BUTTON_X_MAX = CTRL_BAR_BUTTON_X_MIN + CTRL_BAR_BUTTON_WIDTH;

static constexpr int CTRL_BAR_ROW_SPLIT_SPACING_X = BUTTON_SPACING_X;
static constexpr int CTRL_BAR_ROW_SPLIT_WIDTH = CTRL_BAR_BUTTON_WIDTH - 2 * CTRL_BAR_ROW_SPLIT_SPACING_X;
static constexpr int CTRL_BAR_ROW_SPLIT_X_MIN = CTRL_BAR_BUTTON_X_MIN + CTRL_BAR_ROW_SPLIT_SPACING_X;
static constexpr int CTRL_BAR_ROW_SPLIT_X_MAX = CTRL_BAR_ROW_SPLIT_X_MIN + CTRL_BAR_ROW_SPLIT_WIDTH;


CtrlState DrawCtrlBar(CtrlState state, const bool trigger_tree_reset, const bool goal_reached) {
    // Background
    DrawRectangleRec(CTRL_BAR_REC, COLOR_STAT_BAR_BACKGROUND);

    const float CTRL_BAR_BUTTON_HEIGHT = float(CTRL_BAR_ROW_HEIGHT - BUTTON_SPACING_Y);

    int selector_mode_int = static_cast<int>(state.selector_mode);
    const Rectangle selector_mode_bounds = {CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_1_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT};
    GuiToggleGroup(selector_mode_bounds, "Place Goal\nPlace Start\nAdd Obstacle\nRemove Obstacle", &selector_mode_int);
    state.selector_mode = static_cast<SelectorMode>(selector_mode_int);

    static constexpr int CTRL_BAR_ROW_SPLIT_1 = (CTRL_BAR_ROW_5_Y + CTRL_BAR_ROW_6_Y) / 2;
    DrawLineEx({CTRL_BAR_ROW_SPLIT_X_MIN, CTRL_BAR_ROW_SPLIT_1}, {CTRL_BAR_ROW_SPLIT_X_MAX, CTRL_BAR_ROW_SPLIT_1}, BORDER_THICKNESS, COLOR_GRAY_096);

    int tree_growth_mode_int = static_cast<int>(state.tree_growth_mode);
    const Rectangle tree_growth_mode_bounds = {CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_6_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT};
    GuiToggleGroup(tree_growth_mode_bounds, "Play\nUntil Goal Reached\nPause", &tree_growth_mode_int);
    state.tree_growth_mode = static_cast<TreeGrowthMode>(tree_growth_mode_int);

    const bool tree_should_grow_once = GuiButton((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_9_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Advance");
    state.tree_should_reset = GuiButton((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_10_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Reset");
    if (trigger_tree_reset) {
        state.tree_should_reset = true;
    }

    state.tree_should_grow = (state.tree_growth_mode == TreeGrowthMode::ALWAYS) || ((state.tree_growth_mode == TreeGrowthMode::UNTIL_GOAL_REACHED) && !goal_reached) || tree_should_grow_once;

    static constexpr int CTRL_BAR_ROW_SPLIT_2 = (CTRL_BAR_ROW_11_Y + CTRL_BAR_ROW_12_Y) / 2;
    DrawLineEx({CTRL_BAR_ROW_SPLIT_X_MIN, CTRL_BAR_ROW_SPLIT_2}, {CTRL_BAR_ROW_SPLIT_X_MAX, CTRL_BAR_ROW_SPLIT_2}, BORDER_THICKNESS, COLOR_GRAY_096);

    GuiToggle((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_12_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Carryover Path", &state.carryover_path);

    const int SPINNER_LABEL_TEXT_HEIGHT = 32;
    const float SPINNER_LABEL_HEIGHT = float(SPINNER_LABEL_TEXT_HEIGHT);

    // TODO refactor this to a spinner+label function
    // TODO refactor the left+right aligned label+value to a function
    GuiSpinner((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_13_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, NULL, &state.num_carryover_ix, 0, NUM_CARRYOVER_OPTIONS.size() - 1, false);
    GuiSetStyle(DEFAULT, TEXT_SIZE, SPINNER_LABEL_TEXT_HEIGHT);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiLabel((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_14_Y, CTRL_BAR_BUTTON_WIDTH, SPINNER_LABEL_HEIGHT}, "Carryover");
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabel((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_14_Y, CTRL_BAR_BUTTON_WIDTH, SPINNER_LABEL_HEIGHT}, TextFormat("%6d", NUM_CARRYOVER_OPTIONS[state.num_carryover_ix]));
    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiSpinner((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_15_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, NULL, &state.num_samples_ix, 0, NUM_SAMPLES_OPTIONS.size() - 1, false);
    GuiSetStyle(DEFAULT, TEXT_SIZE, SPINNER_LABEL_TEXT_HEIGHT);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
    GuiLabel((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_16_Y, CTRL_BAR_BUTTON_WIDTH, SPINNER_LABEL_HEIGHT}, "Samples");
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
    GuiLabel((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_16_Y, CTRL_BAR_BUTTON_WIDTH, SPINNER_LABEL_HEIGHT}, TextFormat("%6d", NUM_SAMPLES_OPTIONS[state.num_samples_ix]));
    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    GuiToggle((Rectangle){CTRL_BAR_BUTTON_X_MIN+60, CTRL_BAR_ROW_0_Y + 10, CTRL_BAR_BUTTON_WIDTH-120, CTRL_BAR_ROW_HEIGHT-20}, "Snap to Grid", &state.snap_to_grid);
    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);

    // Border
    DrawRectangleLinesEx(CTRL_BAR_REC, 3, COLOR_STAT_BAR_BORDER);

    return state;
}