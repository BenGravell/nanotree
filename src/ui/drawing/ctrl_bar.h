#pragma once

#include <raylib.h>

#include "config.h"
#include "ui/colors.h"
#include "ui/ctrl_state.h"
#include "ui/gui_label.h"

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

static constexpr int CTRL_BAR_VIS_BUTTON_WIDTH = (CTRL_BAR_WIDTH - 4 * BUTTON_SPACING_X) / 3;
static constexpr int CTRL_BAR_VIS_BUTTON_HEIGHT = 2 * CTRL_BAR_ROW_HEIGHT - 2 * BUTTON_SPACING_Y;

void ctrlProblemEdit(CtrlState& state) {
    int selector_mode_int = static_cast<int>(state.selector_mode);
    const Rectangle selector_mode_bounds = {CTRL_BAR_COL_0_X + BUTTON_SPACING_X, CTRL_BAR_ROW_1_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT};

    char icon1[32];
    char icon2[32];
    char icon3[32];
    char icon4[32];

    strcpy(icon1, GuiIconText(ICON_LOCATION, NULL));
    strcpy(icon2, GuiIconText(ICON_FLAG, NULL));
    strcpy(icon3, GuiIconText(ICON_DISK_PLUS, NULL));
    strcpy(icon4, GuiIconText(ICON_DISK_MINUS, NULL));

    const char* icons = TextFormat("%s\n%s\n%s\n%s", icon1, icon2, icon3, icon4);

    // Place Goal, Place Start, Add Obstacle, Remove Obstacle
    GuiToggleGroup(selector_mode_bounds, icons, &selector_mode_int);
    state.selector_mode = static_cast<SelectorMode>(selector_mode_int);

    // Remove All Obstacles
    GuiSetIconScale(SMALL_BUTTON_ICON_SCALE);
    state.reset_obstacles = GuiButton((Rectangle){CTRL_BAR_COL_0_X + BUTTON_SPACING_X, CTRL_BAR_ROW_11_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_ROW_HEIGHT}, GuiIconText(ICON_BIN, NULL));
    GuiSetIconScale(BUTTON_ICON_SCALE);

    // Snap to Grid
    GuiToggle((Rectangle){CTRL_BAR_COL_0_X + BUTTON_SPACING_X, CTRL_BAR_ROW_9_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, GuiIconText(ICON_GRID, NULL), &state.snap_to_grid);
}

void ctrlTreeGrowth(CtrlState& state, const bool trigger_tree_reset, const bool goal_reached) {
    int tree_growth_mode_int = static_cast<int>(state.tree_growth_mode);

    const Rectangle tree_growth_mode_bounds = {CTRL_BAR_COL_1_X + BUTTON_SPACING_X, CTRL_BAR_ROW_1_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT};

    char icon1[32];
    char icon2[32];
    char icon3[32];

    strcpy(icon1, GuiIconText(ICON_PLAYER_PLAY, NULL));
    strcpy(icon2, GuiIconText(ICON_PLAYER_TO_FLAG, NULL));
    strcpy(icon3, GuiIconText(ICON_PLAYER_PAUSE, NULL));

    const char* icons = TextFormat("%s\n%s\n%s", icon1, icon2, icon3);

    // Grow Continuously, Grow to Goal, Pause Growth
    GuiToggleGroup(tree_growth_mode_bounds, icons, &tree_growth_mode_int);
    state.tree_growth_mode = static_cast<TreeGrowthMode>(tree_growth_mode_int);

    // Grow Single Iteration
    const bool explicit_tree_grow = GuiButton((Rectangle){CTRL_BAR_COL_1_X + BUTTON_SPACING_X, CTRL_BAR_ROW_7_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, GuiIconText(ICON_PLAYER_NEXT, NULL));

    // Reset Tree
    const bool explicit_tree_reset = GuiButton((Rectangle){CTRL_BAR_COL_1_X + BUTTON_SPACING_X, CTRL_BAR_ROW_9_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, GuiIconText(ICON_RESTART, NULL));

    // Rewiring Enabled
    GuiSetIconScale(SMALL_BUTTON_ICON_SCALE);
    GuiToggle((Rectangle){CTRL_BAR_COL_1_X + BUTTON_SPACING_X, CTRL_BAR_ROW_11_Y, CTRL_BAR_BUTTON_WIDTH, CTRL_BAR_ROW_HEIGHT}, GuiIconText(ICON_SHUFFLE_FILL, NULL), &state.rewire_enabled);
    GuiSetIconScale(BUTTON_ICON_SCALE);

    // Handle conditions.
    state.tree_should_grow = explicit_tree_grow || (state.tree_growth_mode == TreeGrowthMode::ALWAYS) || ((state.tree_growth_mode == TreeGrowthMode::UNTIL_GOAL_REACHED) && !goal_reached);

    state.tree_should_reset = explicit_tree_reset || trigger_tree_reset;
}

void ctrlTreeSize(CtrlState& state) {
    GuiSetStyle(VALUEBOX, SPINNER_BUTTON_WIDTH, CTRL_BAR_BUTTON_WIDTH);
    GuiSetStyle(DEFAULT, TEXT_SIZE, BIG_TEXT_HEIGHT);

    GuiLabelSpinner((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_12_Y + BUTTON_SPACING_Y, CTRL_BAR_WIDE_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Carry", &state.num_carry_ix, NUM_CARRY_OPTIONS);

    GuiLabelSpinner((Rectangle){CTRL_BAR_BUTTON_X_MIN, CTRL_BAR_ROW_15_Y + BUTTON_SPACING_Y, CTRL_BAR_WIDE_BUTTON_WIDTH, CTRL_BAR_BUTTON_HEIGHT}, "Samples", &state.num_samples_ix, NUM_SAMPLES_OPTIONS);

    GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_HEIGHT);
}

void ctrlVisibility(CtrlState& state) {
    GuiSetIconScale(VISIBILITY_BUTTON_ICON_SCALE);
    // NOTE: Visibility toggles drawn in the Stat Bar
    DrawLineEx({STAT_BAR_X_MIN + 2 * BUTTON_SPACING_X, CTRL_BAR_ROW_16_Y}, {STAT_BAR_X_MAX - 2 * BUTTON_SPACING_X, CTRL_BAR_ROW_16_Y}, BORDER_THICKNESS, COLOR_GRAY_064);

    GuiToggle((Rectangle){STAT_BAR_X_MIN + 0 * CTRL_BAR_VIS_BUTTON_WIDTH + 1 * BUTTON_SPACING_X, CTRL_BAR_ROW_16_Y + BUTTON_SPACING_Y, CTRL_BAR_VIS_BUTTON_WIDTH, CTRL_BAR_VIS_BUTTON_HEIGHT}, GuiIconText(ICON_LINK_BOXES, NULL), &state.visibility.tree);
    GuiToggle((Rectangle){STAT_BAR_X_MIN + 1 * CTRL_BAR_VIS_BUTTON_WIDTH + 2 * BUTTON_SPACING_X, CTRL_BAR_ROW_16_Y + BUTTON_SPACING_Y, CTRL_BAR_VIS_BUTTON_WIDTH, CTRL_BAR_VIS_BUTTON_HEIGHT}, GuiIconText(ICON_LINK_NET, NULL), &state.visibility.path);
    GuiToggle((Rectangle){STAT_BAR_X_MIN + 2 * CTRL_BAR_VIS_BUTTON_WIDTH + 3 * BUTTON_SPACING_X, CTRL_BAR_ROW_16_Y + BUTTON_SPACING_Y, CTRL_BAR_VIS_BUTTON_WIDTH, CTRL_BAR_VIS_BUTTON_HEIGHT}, GuiIconText(ICON_EXPLOSION, NULL), &state.visibility.obstacles);

    GuiSetIconScale(BUTTON_ICON_SCALE);
}

void DrawCtrlBar(CtrlState& state, const bool trigger_tree_reset, const bool goal_reached) {
    // Background
    DrawRectangleRec(CTRL_BAR_REC, COLOR_STAT_BAR_BACKGROUND);

    // Controls
    ctrlProblemEdit(state);
    ctrlTreeGrowth(state, trigger_tree_reset, goal_reached);
    ctrlTreeSize(state);
    ctrlVisibility(state);

    // Border
    DrawRectangleLinesEx(CTRL_BAR_REC, 3, COLOR_STAT_BAR_BORDER);
}