#include <raylib.h>
#include <raymath.h>

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

struct Node {
    const Vector2 pos;
    const std::shared_ptr<Node> parent;
};

int main() {
    InitWindow(800, 800, "nanotree");
    SetTargetFPS(60);
    const Vector2 start = {40, 400};
    Vector2 goal = {760, 400};
    std::vector<Vector2> obstacles;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> path;
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        mouse = {std::clamp(mouse.x, 0.0f, 800.0f), std::clamp(mouse.y, 0.0f, 800.0f)};
        if (IsMouseButtonDown(0)) goal = mouse;
        if (IsMouseButtonDown(1) && !std::ranges::any_of(obstacles, [&](const auto& obstacle) { return Vector2Distance(obstacle, mouse) < 5; })) obstacles.push_back(mouse);
        if (IsMouseButtonDown(2)) obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](const Vector2 obs) { return Vector2Distance(obs, mouse) < 40; }), obstacles.end());
        if (IsMouseButtonDown(0) || IsMouseButtonDown(1) || IsMouseButtonDown(2)) {
            nodes = {std::make_shared<Node>(Node{start, nullptr})};
            for (int i = 0; i <= 1000; ++i) {
                Vector2 pos = (i == 1000) ? goal : Vector2(800 * (float(std::rand()) / RAND_MAX), 800 * (float(std::rand()) / RAND_MAX));
                std::shared_ptr<Node> parent = *std::min_element(nodes.begin(), nodes.end(), [&pos](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, pos) < Vector2Distance(b->pos, pos); });
                pos = Vector2Add(parent->pos, Vector2Scale(Vector2Subtract(pos, parent->pos), std::min(1.0f, 30.0f / Vector2Distance(parent->pos, pos))));
                if (std::any_of(obstacles.begin(), obstacles.end(), [&pos](const auto& obstacle) { return Vector2Distance(obstacle, pos) < 40; })) continue;
                nodes.push_back(std::make_shared<Node>(Node(pos, parent)));
            }
            path.clear();
            std::shared_ptr<Node> node = nodes.back();
            while (node->parent) {
                path.push_back(node);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
        }
        BeginDrawing();
        ClearBackground(BLACK);
        for (auto obstacle : obstacles) DrawCircleV(obstacle, 40, DARKGRAY);
        for (auto node : nodes) if (node->parent) DrawLineEx(node->parent->pos, node->pos, 2, DARKBLUE);
        for (auto node : path) DrawLineEx(node->parent->pos, node->pos, 8, RAYWHITE);
        DrawRectangle(goal.x - 12, goal.y - 12, 24, 24, YELLOW);
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
