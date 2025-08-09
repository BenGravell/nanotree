#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

#include "pride.h"

std::mt19937 rng(std::random_device{}());

static constexpr int ENVIRONMENT_WIDTH = 2000;
static constexpr int ENVIRONMENT_HEIGHT = 1000;
static constexpr int RIBBON_NUM_COLS = 4;
static constexpr int RIBBON_NUM_ROWS = 2;
static constexpr int RIBBON_WIDTH = ENVIRONMENT_WIDTH;
static constexpr int RIBBON_HEIGHT = 200;
static constexpr int RIBBON_COL_WIDTH = RIBBON_WIDTH / RIBBON_NUM_COLS;
static constexpr int RIBBON_ROW_HEIGHT = RIBBON_HEIGHT / RIBBON_NUM_ROWS;
static constexpr int SCREEN_WIDTH = ENVIRONMENT_WIDTH;
static constexpr int SCREEN_HEIGHT = ENVIRONMENT_HEIGHT + RIBBON_HEIGHT;
static constexpr int TEXT_HEIGHT = 40;

static constexpr int OBSTACLE_RADIUS = 100;
static constexpr float DEVIATION_DISTANCE_MAX = 0.6f * OBSTACLE_RADIUS;
static constexpr float RADIUS_OF_CURVATURE_MIN = 0.9f * OBSTACLE_RADIUS;

static constexpr int MOUSE_CENTER_RADIUS = 10;
static constexpr int OBSTACLE_SPACING_MIN = 10;

static constexpr int LINE_WIDTH_PATH = 12;
static constexpr int LINE_WIDTH_TREE = 4;
static constexpr int NODE_WIDTH_PATH = 30;

static constexpr float CHEAP_PARENT_SEARCH_RADIUS = 0.25f * DEVIATION_DISTANCE_MAX;
static constexpr float GOAL_REACHED_RADIUS = 40.0f;

static constexpr float GOAL_SAMPLE_PROBABILITY = 0.02;

static constexpr Color COLOR_GOAL_REACHED = BLUE;
static constexpr Color COLOR_GOAL_NOT_REACHED = RED;
static constexpr Color COLOR_NODE_COUNT = PURPLE;
static constexpr Color COLOR_BACKGROUND = {16, 16, 16, 255};
static constexpr Color COLOR_RIBBON_BACKGROUND = {32, 32, 32, 255};
static constexpr Color COLOR_OBSTACLE = {64, 64, 64, 255};
static constexpr Color COLOR_PATH = RAYWHITE;
static constexpr Color COLOR_KEYMAP = RAYWHITE;

// TODO make static constexpr std::array
const std::vector<int> samples_options = {0, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};

struct Node {
    std::shared_ptr<Node> parent;
    Vector2 pos;
    float cost_to_come;
};

bool insideEnvironment(const Vector2 pos) {
    return (0.0f < pos.x) && (pos.x < ENVIRONMENT_WIDTH) && (0.0f < pos.y) && (pos.y < ENVIRONMENT_HEIGHT);
}

Vector2 clampToEnvironment(const Vector2 pos) {
    return Vector2Clamp(pos, {0, 0}, {ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT});
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

std::shared_ptr<Node> chooseParent(const Vector2 pos, const std::vector<std::shared_ptr<Node>>& nodes) {
    auto cheapest_it = std::min_element(nodes.begin(), nodes.end(),
                                        [&pos](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
                                            float da = Vector2Distance(a->pos, pos);
                                            float db = Vector2Distance(b->pos, pos);
                                            bool a_valid = da < CHEAP_PARENT_SEARCH_RADIUS;
                                            bool b_valid = db < CHEAP_PARENT_SEARCH_RADIUS;

                                            if (a_valid && b_valid)
                                                return (a->cost_to_come + da) < (b->cost_to_come + db);
                                            if (a_valid)
                                                return true;
                                            if (b_valid)
                                                return false;
                                            return false;
                                        });

    if (Vector2Distance((*cheapest_it)->pos, pos) < CHEAP_PARENT_SEARCH_RADIUS) {
        return *cheapest_it;
    }

    return *std::min_element(nodes.begin(), nodes.end(),
                             [&pos](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
                                 return Vector2Distance(a->pos, pos) < Vector2Distance(b->pos, pos);
                             });
}

inline Color fpsColor(const int fps) {
    if (fps < 15) return RED;
    if (fps < 30) return GOLD;
    return BLUE;
}

inline Color mapToColor(float x) {
    x = Remap(x, 0.0f, 1.0f, 0.1f, 0.9f);
    const int idx = std::clamp(static_cast<int>(x * 255.0f + 0.5f), 0, 255);
    const auto& rgb = pride_colormap[idx];
    return Color{rgb[0], rgb[1], rgb[2], 255};
}

inline float normalizeCost(const float c, const float c_goal, const float c_max) {
    float x = 0.0f;
    if (x < c_goal) {
        x = Remap(c / c_goal, 0.0f, 1.0f, 0.0f, 0.5f);
    } else {
        x = Remap((c - c_goal) / (c_max - c_goal), 0.0f, 1.0f, 0.5f, 1.0f);
    }
    return std::clamp(x, 0.0f, 1.0f);
}

void DrawSelector(const Vector2 pos) {
    static constexpr float selector_orbit_period_sec = 5.0f;
    static constexpr int num_segments = 12;
    static constexpr int delta_angle = 360 / (2 * num_segments);
    static constexpr int ring_outer_radius = OBSTACLE_RADIUS;
    static constexpr int ring_width = 0.2 * OBSTACLE_RADIUS;
    static constexpr int ring_inner_radius = ring_outer_radius - ring_width;

    const float current_time = GetTime();
    const float offset_angle = fmodf(current_time / selector_orbit_period_sec, 1.0f) * 360.0f;
    DrawCircleV(pos, OBSTACLE_RADIUS, Fade(GRAY, 0.4f));
    DrawRing(pos, ring_outer_radius - ring_width, ring_outer_radius, 0, 360, 0, Fade(GRAY, 0.6f));
    for (int i = 0; i < num_segments; ++i) {
        const float start_angle = 2 * i * delta_angle + offset_angle;
        DrawRing(pos, ring_inner_radius, ring_outer_radius, start_angle, start_angle + delta_angle, 0, LIGHTGRAY);
    }
}

int main() {
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "nanotree");

    // TODO const goal init
    Vector2 goal = {1000, 500};

    std::vector<Vector2> obstacles = {{820, 260}, {820, 380}, {820, 500}, {820, 620}, {820, 740}, {940, 260}, {940, 740}, {1060, 260}, {1060, 740}, {1180, 260}, {1180, 380}, {1180, 620}, {1180, 740}};

    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> path;

    int samples = 2000;

    while (!WindowShouldClose()) {
        if (int scroll = GetMouseWheelMove()) samples = samples_options[std::clamp(int(std::lower_bound(samples_options.begin(), samples_options.end(), samples) - samples_options.begin()) + ((scroll > 0) - (scroll < 0)), 0, int(samples_options.size() - 1))];

        Vector2 mouse = clampToEnvironment(GetMousePosition());

        const bool is_down_lmb = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        const bool is_down_rmb = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
        const bool is_down_mmb = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);
        const bool is_down_mb = is_down_lmb || is_down_rmb || is_down_mmb;

        if (is_down_lmb) {
            goal = mouse;
        }

        if (is_down_rmb) {
            if (std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o) { return Vector2Distance(o, mouse) < OBSTACLE_SPACING_MIN; })) {
                obstacles.push_back(mouse);
            }
        }

        if (is_down_mmb) {
            obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, mouse) < OBSTACLE_RADIUS; }), obstacles.end());
        }

        const bool do_update = is_down_mb || nodes.empty();
        if (do_update) {
            // TODO const start node
            nodes = {std::make_shared<Node>(Node{nullptr, {100, 500}, 0.0f})};

            for (int i = 0; i <= samples; ++i) {
                Vector2 pos = (i == samples) ? goal : sample(goal);

                std::shared_ptr<Node> parent = chooseParent(pos, nodes);

                pos = clampToEnvironment(pos);
                pos = attractByDistance(pos, parent);
                pos = attractByAngle(pos, parent);

                if (!insideEnvironment(pos)) {
                    continue;
                }

                if (std::any_of(obstacles.begin(), obstacles.end(), [&pos](auto& obs) { return Vector2Distance(obs, pos) < OBSTACLE_RADIUS; })) {
                    continue;
                }

                const float cost = Vector2Distance(parent->pos, pos);
                nodes.push_back(std::make_shared<Node>(Node{parent, pos, parent->cost_to_come + cost}));
            }

            // TODO func extractPath(nodes, goal)
            path.clear();
            std::shared_ptr<Node> node = *std::min_element(nodes.begin(), nodes.end(), [&goal](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, goal) < Vector2Distance(b->pos, goal); });
            while (node->parent) {
                path.push_back(node);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
        }

        float cost_to_come_goal = 0.0f;
        for (auto node : path) {
            cost_to_come_goal = std::max(cost_to_come_goal, node->cost_to_come);
        }
        float cost_to_come_max = 0.0f;
        for (auto node : nodes) {
            cost_to_come_max = std::max(cost_to_come_max, node->cost_to_come);
        }

        const bool goal_reached = Vector2Distance(path.empty() ? nodes.front()->pos : path.back()->pos, goal) < GOAL_REACHED_RADIUS;

        const int fps = GetFPS();

        // DRAWING SECTION
        BeginDrawing();
        DrawRectangle(0, 0, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT, COLOR_BACKGROUND);

        for (const Vector2 obstacle : obstacles) {
            DrawCircleV(obstacle, OBSTACLE_RADIUS, COLOR_OBSTACLE);
        }

        // TODO func DrawTree
        for (const auto node : nodes) {
            if (!node->parent) {
                continue;
            }
            const Color color = mapToColor(normalizeCost(node->cost_to_come, cost_to_come_goal, cost_to_come_max));
            DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_TREE, color);
        }

        // TODO func DrawPath
        for (const auto node : path) {
            DrawLineEx(node->parent->pos, node->pos, LINE_WIDTH_PATH, COLOR_PATH);
            DrawCircleV(node->pos, NODE_WIDTH_PATH / 2, COLOR_PATH);
        }

        DrawSelector(mouse);
        DrawCircleV(mouse, MOUSE_CENTER_RADIUS, LIGHTGRAY);

        const Color goal_color = goal_reached ? COLOR_GOAL_REACHED : COLOR_GOAL_NOT_REACHED;
        DrawCircleV(goal, GOAL_REACHED_RADIUS, Fade(goal_color, 0.8f));

        // Ribbon
        DrawRectangle(0, ENVIRONMENT_HEIGHT, ENVIRONMENT_WIDTH, RIBBON_HEIGHT, COLOR_RIBBON_BACKGROUND);

        for (int y = ENVIRONMENT_HEIGHT; y < SCREEN_HEIGHT; y += RIBBON_ROW_HEIGHT) {
            for (int x = 0; x < ENVIRONMENT_WIDTH; x += RIBBON_COL_WIDTH) {
                DrawRectangleLines(x, y, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT, LIGHTGRAY);
            }
        }

        static constexpr int TEXT_MARGIN_WIDTH = 20;

        static constexpr int RIBBON_ROW_1_Y = ENVIRONMENT_HEIGHT + 0 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT) / 2;
        static constexpr int RIBBON_ROW_2_Y = ENVIRONMENT_HEIGHT + 1 * RIBBON_ROW_HEIGHT + (RIBBON_ROW_HEIGHT - TEXT_HEIGHT) / 2;

        DrawText(std::string(goal_reached ? "Goal reached" : "Goal not reached").c_str(), 0 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y, TEXT_HEIGHT, goal_color);
        DrawText(TextFormat("%2i FPS", fps), 1 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y, TEXT_HEIGHT, fpsColor(fps));
        DrawText((std::to_string(nodes.size()) + " nodes").c_str(), 2 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y, TEXT_HEIGHT, COLOR_NODE_COUNT);
        DrawText((std::to_string(samples) + " samples").c_str(), 3 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_1_Y, TEXT_HEIGHT, COLOR_NODE_COUNT);

        DrawText("[LMB] move goal", 0 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT, COLOR_KEYMAP);
        DrawText("[RMB] insert obstacle", 1 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT, COLOR_KEYMAP);
        DrawText("[MMB] delete obstacle", 2 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT, COLOR_KEYMAP);
        DrawText("[Scroll] # samples", 3 * 500 + TEXT_MARGIN_WIDTH, RIBBON_ROW_2_Y, TEXT_HEIGHT, COLOR_KEYMAP);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
