#include "Game.hpp"
#include "Constants.hpp"

// ── Test map layout ──────────────────────────────────────────
// 0 = grass, 1 = water, 2 = tree, 3 = path
static const std::vector<std::vector<int>> MAP_DATA = {
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2 },
    { 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 1, 1, 1, 1, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2 },
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
};

Game::Game() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    // Map
    int mapW = (int)MAP_DATA[0].size();
    int mapH = (int)MAP_DATA.size();
    m_map = std::make_unique<TileMap>(mapW, mapH);
    m_map->LoadFromArray(MAP_DATA);

    // Player — start in middle of map
    float startX = (mapW / 2) * TILE_SIZE;
    float startY = (mapH / 2) * TILE_SIZE;
    m_player = std::make_unique<Player>(startX, startY);

    // Camera
    m_camera = {};
    m_camera.zoom   = 1.5f;
    m_camera.offset = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    m_camera.target = m_player->GetCenter();
    m_camera.rotation = 0.0f;
}

Game::~Game() {
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Update(dt);
        Draw();
    }
}

void Game::Update(float dt) {
    m_player->Update(dt, *m_map);

    // Smooth camera follow
    Vector2 center = m_player->GetCenter();
    m_camera.target.x += (center.x - m_camera.target.x) * 8.0f * dt;
    m_camera.target.y += (center.y - m_camera.target.y) * 8.0f * dt;
}

void Game::Draw() {
    BeginDrawing();
        ClearBackground({ 20, 20, 20, 255 });

        BeginMode2D(m_camera);
            m_map->Draw(m_camera);
            m_player->Draw();
        EndMode2D();

        DrawHUD();
    EndDrawing();
}

void Game::DrawHUD() {
    // FPS counter
    DrawFPS(8, 8);

    // Controls hint
    DrawText("WASD / Arrow keys to move", 8, SCREEN_HEIGHT - 24, 16, LIGHTGRAY);
}
