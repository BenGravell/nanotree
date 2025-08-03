#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

int main() {
  struct Node { Vector2 pos; std::shared_ptr<Node> parent; };
  SetTraceLogLevel(LOG_ERROR); 
  InitWindow(1000, 600, "nanotree");
  Vector2 goal = {500, 250};
  std::vector<Vector2> obstacles = {{380, 130}, {380, 190}, {380, 250}, {380, 310}, {380, 370}, {440, 130}, {440, 370}, {500, 130}, {500, 370}, {560, 130}, {560, 370}, {620, 130}, {620, 190}, {620, 310}, {620, 370}};
  std::vector<std::shared_ptr<Node>> nodes;
  std::vector<std::shared_ptr<Node>> path;
  int samples = 1000;
  std::vector<int> samples_options = {0, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000};
  while (!WindowShouldClose()) {
    if (int scroll = GetMouseWheelMove()) samples = samples_options[std::clamp(int(std::lower_bound(samples_options.begin(), samples_options.end(), samples) - samples_options.begin()) + ((scroll > 0) - (scroll < 0)), 0, int(samples_options.size() - 1))];
    Vector2 mouse = {std::clamp(GetMousePosition().x, 0.0f, 1000.0f), std::clamp(GetMousePosition().y, 0.0f, 500.0f)};
    if (IsMouseButtonDown(0)) goal = mouse;
    if (IsMouseButtonDown(1) && std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o){ return Vector2Distance(o, mouse) < 5; })) obstacles.push_back(mouse);
    if (IsMouseButtonDown(2)) obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, mouse) < 50; }), obstacles.end());
    if (IsMouseButtonDown(0) || IsMouseButtonDown(1) || IsMouseButtonDown(2) || nodes.empty()) {
      nodes = {std::make_shared<Node>(Node{{50, 250}, nullptr})};
      for (int i = 0; i <= samples; ++i) {
        Vector2 pos = (i == samples) ? goal : Vector2{1000 * float(std::rand()) / RAND_MAX, 500 * float(std::rand()) / RAND_MAX};
        std::shared_ptr<Node> parent = *std::min_element(nodes.begin(), nodes.end(), [&pos](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, pos) < Vector2Distance(b->pos, pos); });
        pos = Vector2Add(parent->pos, Vector2Scale(Vector2Subtract(pos, parent->pos), std::min(1.0f, 40.0f / Vector2Distance(parent->pos, pos))));
        if (!std::any_of(obstacles.begin(), obstacles.end(), [&pos](auto& obs) { return Vector2Distance(obs, pos) < 50; })) nodes.push_back(std::make_shared<Node>(Node{pos, parent}));
      }
      path.clear();
      std::shared_ptr<Node> node = *std::min_element(nodes.begin(), nodes.end(), [&goal](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, goal) < Vector2Distance(b->pos, goal); });
      while (node->parent) {
        path.push_back(node);
        node = node->parent;
      }
      std::reverse(path.begin(), path.end());
    }
    bool solved = Vector2Distance(path.back()->pos, goal) < 20;
    BeginDrawing();
    DrawRectangle(0, 0, 1000, 500, Fade(BLACK, 0.1f));
    DrawRing(mouse, 45, 50, 0, 360, 0, DARKGRAY);
    DrawCircleV(mouse, 5, LIGHTGRAY);
    for (auto obstacle : obstacles) DrawCircleV(obstacle, 50, DARKGRAY);
    for (auto node : nodes)
      if (node->parent) DrawLineEx(node->parent->pos, node->pos, 2, BLUE);
    for (auto node : path) DrawLineEx(node->parent->pos, node->pos, 8, RAYWHITE);
    DrawRectangle(goal.x - 12, goal.y - 12, 24, 24, solved ? GOLD : RED);
    DrawRectangle(0, 500, 1000, 100, {40, 40, 40, 255});
    for (int y = 500; y < 600; y += 50) for (int x = 0; x < 1000; x += 250) DrawRectangleLines(x, y, 250, 50, LIGHTGRAY);
    DrawText(std::string(solved ? "Reached Goal" : "Did Not Reach Goal").c_str(), 10, 515, 20, solved ? GOLD : RED);
    DrawFPS(260, 515);
    DrawText((std::to_string(nodes.size()) + " nodes").c_str(), 510, 515, 20, BLUE);
    DrawText((std::to_string(samples) + " samples").c_str(), 760, 515, 20, BLUE);
    DrawText("[LMB] move goal", 10, 565, 20, RAYWHITE);
    DrawText("[RMB] insert obstacle", 260, 565, 20, RAYWHITE);
    DrawText("[MMB] delete obstacle", 510, 565, 20, RAYWHITE);
    DrawText("[Scroll] # samples", 760, 565, 20, RAYWHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
