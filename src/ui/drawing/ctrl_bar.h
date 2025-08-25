#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"
#include "ui/selector_mode.h"

static constexpr int CTRL_BAR_X_MIN = ENVIRONMENT_X_MAX;
static constexpr int CTRL_BAR_X_MAX = CTRL_BAR_X_MIN + CTRL_BAR_WIDTH;
static constexpr int CTRL_BAR_Y_MIN = 0;
static constexpr int CTRL_BAR_Y_MAX = CTRL_BAR_Y_MIN + CTRL_BAR_HEIGHT;
static constexpr Rectangle CTRL_BAR_REC = {CTRL_BAR_X_MIN, CTRL_BAR_Y_MIN, CTRL_BAR_WIDTH, CTRL_BAR_HEIGHT};

enum class TreeGrowthMode {
    ALWAYS = 0,
    ON_TRIGGER = 1,
    NEVER = 2
};

struct CtrlState {
    SelectorMode selector_mode = SelectorMode::PLACE_GOAL;
    TreeGrowthMode tree_growth_mode = TreeGrowthMode::ON_TRIGGER;
    bool tree_should_reset = false;
    bool tree_should_grow = false;
    bool carryover_path = true;
    int num_samples = 200;
    int num_carryover = 2000;
    bool num_samples_edit_mode = false;
    bool num_carryover_edit_mode = false;
};

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

static constexpr int CTRL_BAR_ROW_SPLIT_MARGIN = 15;

static constexpr int CTRL_BAR_ROW_SPLIT_4_5 = (CTRL_BAR_ROW_4_Y + CTRL_BAR_ROW_5_Y) / 2;
static constexpr int CTRL_BAR_ROW_SPLIT_11_12 = (CTRL_BAR_ROW_11_Y + CTRL_BAR_ROW_12_Y) / 2;

CtrlState DrawCtrlBar(CtrlState state, const bool trigger_tree_reset, const bool trigger_tree_growth) {
    // Background
    DrawRectangleRec(CTRL_BAR_REC, COLOR_STAT_BAR_BACKGROUND);

    int selector_mode_int = static_cast<int>(state.selector_mode);
    GuiToggleGroup((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_0_Y, CTRL_BAR_WIDTH, CTRL_BAR_ROW_HEIGHT}, "Place Start\nPlace Goal\nAdd Obstacle\nRemove Obstacle", &selector_mode_int);
    state.selector_mode = static_cast<SelectorMode>(selector_mode_int);

    DrawLineEx({CTRL_BAR_X_MIN + CTRL_BAR_ROW_SPLIT_MARGIN, CTRL_BAR_ROW_SPLIT_4_5}, {CTRL_BAR_X_MAX - CTRL_BAR_ROW_SPLIT_MARGIN, CTRL_BAR_ROW_SPLIT_4_5}, 3, COLOR_GRAY_096);

    int tree_growth_mode_int = static_cast<int>(state.tree_growth_mode);
    GuiToggleGroup((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_5_Y, CTRL_BAR_WIDTH, CTRL_BAR_ROW_HEIGHT}, "Grow Always\nGrow On Trigger\nGrow Never", &tree_growth_mode_int);
    state.tree_growth_mode = static_cast<TreeGrowthMode>(tree_growth_mode_int);

    const bool tree_should_grow_once = GuiButton((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_9_Y, CTRL_BAR_WIDTH, CTRL_BAR_ROW_HEIGHT}, "Grow Once");
    state.tree_should_reset = GuiButton((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_10_Y, CTRL_BAR_WIDTH, CTRL_BAR_ROW_HEIGHT}, "Reset Tree");
    if (trigger_tree_reset) {
        state.tree_should_reset = true;
    }

    state.tree_should_grow = (state.tree_growth_mode == TreeGrowthMode::ALWAYS) || ((state.tree_growth_mode == TreeGrowthMode::ON_TRIGGER) && trigger_tree_growth) || tree_should_grow_once;

    DrawLineEx({CTRL_BAR_X_MIN + CTRL_BAR_ROW_SPLIT_MARGIN, CTRL_BAR_ROW_SPLIT_11_12}, {CTRL_BAR_X_MAX - CTRL_BAR_ROW_SPLIT_MARGIN, CTRL_BAR_ROW_SPLIT_11_12}, 3, COLOR_GRAY_096);

    GuiToggle((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_12_Y, CTRL_BAR_WIDTH, CTRL_BAR_ROW_HEIGHT}, "Carryover Path", &state.carryover_path);

    // TODO use NUM_CARRYOVER_OPTIONS and NUM_SAMPLES_OPTIONS in spinners

    if (GuiSpinner((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_13_Y, CTRL_BAR_WIDTH, CTRL_BAR_ROW_HEIGHT}, NULL, &state.num_carryover, 0, 10000, state.num_carryover_edit_mode)) {
        state.num_carryover_edit_mode = !state.num_carryover_edit_mode;
    }
    GuiLabel((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_14_Y, CTRL_BAR_WIDTH, float(GuiGetStyle(DEFAULT, TEXT_SIZE) + 10)}, "Samples");

    if (GuiSpinner((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_15_Y, CTRL_BAR_WIDTH, CTRL_BAR_ROW_HEIGHT}, NULL, &state.num_samples, 0, 50000, state.num_samples_edit_mode)) {
        state.num_samples_edit_mode = !state.num_samples_edit_mode;
    }
    GuiLabel((Rectangle){CTRL_BAR_X_MIN, CTRL_BAR_ROW_16_Y, CTRL_BAR_WIDTH, float(GuiGetStyle(DEFAULT, TEXT_SIZE) + 10)}, "Carryover");


    // Border
    DrawRectangleLinesEx(CTRL_BAR_REC, 3, COLOR_STAT_BAR_BORDER);

    return state;
}