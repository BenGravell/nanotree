#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

int main() {
  struct Node { Vector2 pos; std::shared_ptr<Node> parent; };
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(2000, 1000+200, "nanotree");
  Vector2 goal = {1000, 500};
  std::vector<Vector2> obstacles = { {820, 260}, {820, 380}, {820, 500}, {820, 620}, {820, 740}, {940, 260}, {940, 740}, {1060, 260}, {1060, 740}, {1180, 260}, {1180, 380}, {1180, 620}, {1180, 740} };
  std::vector<std::shared_ptr<Node>> nodes;
  std::vector<std::shared_ptr<Node>> path;
  int samples = 2000;
  std::vector<int> samples_options = {0, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000};
  while (!WindowShouldClose()) {
    if (int scroll = GetMouseWheelMove()) samples = samples_options[std::clamp(int(std::lower_bound(samples_options.begin(), samples_options.end(), samples) - samples_options.begin()) + ((scroll > 0) - (scroll < 0)), 0, int(samples_options.size() - 1))];
    Vector2 mouse = Vector2Clamp(GetMousePosition(), {0, 0}, {2000, 1000});
    if (IsMouseButtonDown(0)) goal = mouse;
    if (IsMouseButtonDown(1) && std::none_of(obstacles.begin(), obstacles.end(), [&](auto& o){ return Vector2Distance(o, mouse) < 10; })) obstacles.push_back(mouse);
    if (IsMouseButtonDown(2)) obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(), [&](Vector2 o) { return Vector2Distance(o, mouse) < 100; }), obstacles.end());
    if (IsMouseButtonDown(0) || IsMouseButtonDown(1) || IsMouseButtonDown(2) || nodes.empty()) {
      nodes = {std::make_shared<Node>(Node{{100, 500}, nullptr})};
      for (int i = 0; i <= samples; ++i) {
        Vector2 pos = (i == samples) ? goal : Vector2{2000 * float(std::rand()) / RAND_MAX, 1000 * float(std::rand()) / RAND_MAX};
        std::shared_ptr<Node> parent = *std::min_element(nodes.begin(), nodes.end(), [&pos](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, pos) < Vector2Distance(b->pos, pos); });
        Vector2 sdir = Vector2Normalize(Vector2Subtract(pos, parent->pos));
        Vector2 pdir = parent->parent ? Vector2Normalize(Vector2Subtract(parent->pos, parent->parent->pos)) : sdir;
        float angle = std::clamp(Vector2Angle(pdir, sdir), -30.0f * DEG2RAD, 30.0f * DEG2RAD);
        sdir = Vector2Rotate(pdir, angle);
        pos = Vector2Add(parent->pos, Vector2Scale(sdir, std::min(80.0f, Vector2Distance(pos, parent->pos))));
        pos = Vector2Clamp(pos, {0, 0}, {2000, 1000});
        if (!std::any_of(obstacles.begin(), obstacles.end(), [&pos](auto& obs) { return Vector2Distance(obs, pos) < 100; })) nodes.push_back(std::make_shared<Node>(Node{pos, parent}));
      }
      path.clear();
      std::shared_ptr<Node> node = *std::min_element(nodes.begin(), nodes.end(), [&goal](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) { return Vector2Distance(a->pos, goal) < Vector2Distance(b->pos, goal); });
      while (node->parent) {
        path.push_back(node);
        node = node->parent;
      }
      std::reverse(path.begin(), path.end());
    }
    bool solved = Vector2Distance(path.empty() ? nodes.front()->pos : path.back()->pos, goal) < 40;
    BeginDrawing();
    DrawRectangle(0, 0, 2000, 1000, Fade(BLACK, 0.2f));
    for (auto obstacle : obstacles) DrawCircleV(obstacle, 100, DARKGRAY);
    for (auto node : nodes)
      if (node->parent) DrawLineEx(node->parent->pos, node->pos, 2, PURPLE);
    for (auto node : path) { DrawLineEx(node->parent->pos, node->pos, 10, RAYWHITE); DrawCircleV(node->pos, 10, RAYWHITE); }
    DrawRing(mouse, 100-10, 100, 0, 360, 0, DARKGRAY);
    DrawCircleV(mouse, 10, LIGHTGRAY);
    DrawRectangle(goal.x - 25, goal.y - 25, 50, 50, solved ? BLUE : RED);
    DrawRectangle(0, 1000, 2000, 200, {40, 40, 40, 255});
    for (int y = 1000; y < 1200; y += 100) for (int x = 0; x < 2000; x += 500) DrawRectangleLines(x, y, 500, 100, LIGHTGRAY);
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
  }
  CloseWindow();
  return 0;
}
