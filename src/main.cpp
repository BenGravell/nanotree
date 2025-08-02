#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

int main() {
    struct Node { Vector2 pos; std::shared_ptr<Node> parent; };
    InitWindow(800, 900, "nanotree");
    Vector2 goal = {750, 400};
    std::vector<Vector2> obstacles = {{400, 400}};
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> path;
    int num_nodes = 1000;
    const std::vector<int> schedule = {0, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000};
    while (!WindowShouldClose()) {
        if (int scroll = GetMouseWheelMove()) num_nodes = schedule[std::clamp(int(std::lower_bound(schedule.begin(), schedule.end(), num_nodes) - schedule.begin()) + ((scroll > 0) - (scroll < 0)), 0, int(schedule.size() - 1))];
        Vector2 mouse = {std::clamp(GetMousePosition().x, 0.0f, 800.0f), std::clamp(GetMousePosition().y, 0.0f, 800.0f)};
        if (IsMouseButtonDown(0)) goal = mouse;
        if (IsMouseButtonDown(1) && !std::ranges::any_of(obstacles, [&](auto& o) { return Vector2Distance(o, mouse) < 5; })) obstacles.push_back(mouse);
        if (IsMouseButtonDown(2)) obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, mouse) < 50; }), obstacles.end());
        if (IsMouseButtonDown(0) || IsMouseButtonDown(1) || IsMouseButtonDown(2) || nodes.empty()) {
            nodes = {std::make_shared<Node>(Node{{50, 400}, nullptr})};
            for (int i = 0; i <= num_nodes; ++i) {
                Vector2 pos = (i == num_nodes) ? goal : Vector2(800 * float(std::rand()) / RAND_MAX, 800 * float(std::rand()) / RAND_MAX);
                std::shared_ptr<Node> parent = *std::min_element(nodes.begin(), nodes.end(), [&pos](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, pos) < Vector2Distance(b->pos, pos); });
                pos = Vector2Add(parent->pos, Vector2Scale(Vector2Subtract(pos, parent->pos), std::min(1.0f, 20.0f / Vector2Distance(parent->pos, pos))));
                if (!std::any_of(obstacles.begin(), obstacles.end(), [&pos](auto& obs) { return Vector2Distance(obs, pos) < 50; })) nodes.push_back(std::make_shared<Node>(pos, parent));
            }
            path.clear();
            std::shared_ptr<Node> node = *std::min_element(nodes.begin(), nodes.end(), [&goal](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, goal) < Vector2Distance(b->pos, goal); });
            while (node->parent) {
                path.push_back(node);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
        }
        BeginDrawing();
        DrawRectangle(0, 0, 800, 900, Fade(BLACK, 0.1f));
        DrawCircleLinesV(mouse, 50, DARKGRAY);
        DrawCircleV(mouse, 5, LIGHTGRAY);
        for (auto obstacle : obstacles) DrawCircleV(obstacle, 50, DARKGRAY);
        for (auto node : nodes)
            if (node->parent) DrawLineEx(node->parent->pos, node->pos, 2, BLUE);
        for (auto node : path) DrawLineEx(node->parent->pos, node->pos, 8, RAYWHITE);
        DrawRectangle(goal.x - 12, goal.y - 12, 24, 24, GOLD);
        DrawRectangle(0, 800, 800, 100, {40, 40, 40, 255});
        DrawText("[LMB] move goal  [RMB] add obstacle  [MMB] del obstacle  [Scroll] # samples", 10, 860, 20, RAYWHITE);
        DrawText((std::to_string(num_nodes) + " samples").c_str(), 180, 820, 20, DARKBLUE);
        DrawText((std::to_string(nodes.size()) + " nodes").c_str(), 380, 820, 20, BLUE);
        DrawText((std::string("Reached goal: ") + (Vector2Distance(path.back()->pos, goal) < 20 ? "True" : "False")).c_str(), 560, 820, 20, GOLD);
        DrawFPS(10, 820);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
