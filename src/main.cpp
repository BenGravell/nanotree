#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

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

static constexpr int OBSTACLE_RADIUS = 100;

// TODO make static constexpr std::array
const std::vector<int> samples_options = {0, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};

struct Node {
    Vector2 pos;
    std::shared_ptr<Node> parent;
};

Vector2 clampToEnvironment(const Vector2 pos) {
    return Vector2Clamp(pos, {0, 0}, {ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT});
}

Vector2 sample() {
    static std::uniform_real_distribution<float> dist_x(0.0f, ENVIRONMENT_WIDTH);
    static std::uniform_real_distribution<float> dist_y(0.0f, ENVIRONMENT_HEIGHT);
    return Vector2{dist_x(rng), dist_y(rng)};
}

static constexpr float DEVIATION_DISTANCE_MAX = 80.0f;
static constexpr float DEVIATION_ANGLE_MAX = 30.0f * DEG2RAD;
static constexpr float RADIUS_OF_CURVATURE_MIN = 1.2f * OBSTACLE_RADIUS;

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

    const float angle = std::clamp(Vector2Angle(direction_xy, direction_yz), -deviation_angle_max, deviation_angle_max);

    const Vector2 direction_out = Vector2Rotate(direction_xy, angle);

    return Vector2Add(parent->pos, direction_out * distance_yz);
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

    float last_time = GetTime();
    float current_time = GetTime();

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

        if (is_down_rmb && std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o) { return Vector2Distance(o, mouse) < 10; })) {
            obstacles.push_back(mouse);
        }

        if (is_down_mmb) {
            obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, mouse) < OBSTACLE_RADIUS; }), obstacles.end());
        }
        const bool do_update = is_down_mb || nodes.empty();
        if (do_update) {
            // TODO const start node
            nodes = {std::make_shared<Node>(Node{{100, 500}, nullptr})};

            for (int i = 0; i <= samples; ++i) {
                Vector2 pos = (i == samples) ? goal : sample();

                std::shared_ptr<Node> parent = *std::min_element(nodes.begin(), nodes.end(), [&pos](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, pos) < Vector2Distance(b->pos, pos); });

                pos = attractByDistance(pos, parent);
                pos = attractByAngle(pos, parent);
                pos = clampToEnvironment(pos);

                if (std::any_of(obstacles.begin(), obstacles.end(), [&pos](auto& obs) { return Vector2Distance(obs, pos) < OBSTACLE_RADIUS; })) {
                    continue;
                }

                nodes.push_back(std::make_shared<Node>(Node{pos, parent}));
            }

            path.clear();
            std::shared_ptr<Node> node = *std::min_element(nodes.begin(), nodes.end(), [&goal](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, goal) < Vector2Distance(b->pos, goal); });
            while (node->parent) {
                path.push_back(node);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
        }

        // TODO 40 to goal tolerance
        bool solved = Vector2Distance(path.empty() ? nodes.front()->pos : path.back()->pos, goal) < 40;

        // DRAWING SECTION
        BeginDrawing();
        DrawRectangle(0, 0, ENVIRONMENT_WIDTH, ENVIRONMENT_HEIGHT, BLACK);
        for (auto obstacle : obstacles) DrawCircleV(obstacle, OBSTACLE_RADIUS, DARKGRAY);
        for (auto node : nodes)
            if (node->parent) DrawLineEx(node->parent->pos, node->pos, 2, PURPLE);
        for (auto node : path) {
            DrawLineEx(node->parent->pos, node->pos, 10, RAYWHITE);
            DrawCircleV(node->pos, 10, RAYWHITE);
        }

        current_time = GetTime();

        // ring selector orbiter
        static constexpr float selector_orbit_period = 5.0f;
        static constexpr int num_segments = 10;
        static constexpr int delta_angle = 360 / (2 * num_segments);
        static constexpr int ring_outer_radius = OBSTACLE_RADIUS;
        static constexpr int ring_width = 10;
        const int offset_angle = fmodf(current_time / selector_orbit_period, 1.0f) * 360;
        const int offset_angle2 = fmodf(-current_time / selector_orbit_period, 1.0f) * 360;
        DrawCircleV(mouse, OBSTACLE_RADIUS, Fade(GRAY, 0.4f));
        DrawRing(mouse, ring_outer_radius - ring_width, ring_outer_radius, 0, 360, 0, Fade(GRAY, 0.6f));
        for (int i = 0; i < num_segments; ++i) {
            const int start_angle = 2 * i * delta_angle + offset_angle;
            const int start_angle2 = 2 * i * delta_angle + offset_angle2;
            DrawRing(mouse, ring_outer_radius - ring_width / 2, ring_outer_radius, start_angle, start_angle + delta_angle, 0, LIGHTGRAY);
            DrawRing(mouse, ring_outer_radius - ring_width, ring_outer_radius - ring_width / 2, start_angle2, start_angle2 + delta_angle, 0, LIGHTGRAY);
        }

        DrawCircleV(mouse, 10, LIGHTGRAY);
        DrawRectangle(goal.x - 25, goal.y - 25, 50, 50, solved ? BLUE : RED);
        DrawRectangle(0, ENVIRONMENT_HEIGHT, ENVIRONMENT_WIDTH, 200, {40, 40, 40, 255});
        for (int y = ENVIRONMENT_HEIGHT; y < SCREEN_HEIGHT; y += RIBBON_ROW_HEIGHT) {
            for (int x = 0; x < ENVIRONMENT_WIDTH; x += RIBBON_COL_WIDTH) {
                DrawRectangleLines(x, y, RIBBON_COL_WIDTH, RIBBON_ROW_HEIGHT, LIGHTGRAY);
            }
        }
        DrawText(std::string(solved ? "Reached Goal" : "Did Not Reach Goal").c_str(), 20, 1030, 40, solved ? BLUE : RED);
        int fps = GetFPS();
        DrawText(TextFormat("%2i FPS", fps), 520, 1030, 40, (fps < 15) ? RED : ((fps < 30) ? GOLD : BLUE));
        DrawText((std::to_string(nodes.size()) + " nodes").c_str(), 1020, 1030, 40, PURPLE);
        DrawText((std::to_string(samples) + " samples").c_str(), 1520, 1030, 40, PURPLE);

        DrawText("[LMB] move goal", 20, 1130, 40, RAYWHITE);
        DrawText("[RMB] insert obstacle", 520, 1130, 40, RAYWHITE);
        DrawText("[MMB] delete obstacle", 1020, 1130, 40, RAYWHITE);
        DrawText("[Scroll] # samples", 1520, 1130, 40, RAYWHITE);

        EndDrawing();

        last_time = current_time;
    }

    CloseWindow();
    return 0;
}
