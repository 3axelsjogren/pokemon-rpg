#include "Game.h"
#include "Constants.h"

Game::Game() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    m_mapManager = std::make_unique<MapManager>();

    m_player = std::make_unique<Player>(10 * TILE_SIZE, 10 * TILE_SIZE);

    m_camera = {};
    m_camera.zoom = 1.5f;
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
    TileMap* map = m_mapManager->GetCurrentMap();
    m_player->Update(dt, *map);

    // Kolla dörrtrigger
    float spawnX, spawnY;
    std::string targetMap;
    Vector2 center = m_player->GetCenter();
    if (m_mapManager->CheckDoorTrigger(center.x, center.y, spawnX, spawnY, targetMap)) {
        m_mapManager->SwitchMap(targetMap, spawnX, spawnY);
        m_player->SetPosition(spawnX, spawnY);
        m_camera.target = m_player->GetCenter();
    }

    // Smooth camera
    center = m_player->GetCenter();
    m_camera.target.x += (center.x - m_camera.target.x) * 8.0f * dt;
    m_camera.target.y += (center.y - m_camera.target.y) * 8.0f * dt;

    // NPC-interaktion
    if (IsKeyPressed(KEY_E)) {
        if (m_dialog.IsOpen()) {
            m_dialog.Close();
        } else {
            Vector2 pos = m_player->GetPosition();
            for (auto& npc : m_mapManager->GetNPCs()) {
                if (npc.IsNearPlayer(pos.x, pos.y)) {
                    m_dialog.Show(npc.GetName(), npc.GetDialog());
                    break;
                }
            }
        }
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground({ 20, 20, 20, 255 });
    BeginMode2D(m_camera);
    m_mapManager->GetCurrentMap()->Draw(m_camera);
    for (auto& npc : m_mapManager->GetNPCs())
        npc.Draw();
    m_player->Draw();
    EndMode2D();
    m_dialog.Draw();
    DrawHUD();
    EndDrawing();
}

void Game::DrawHUD() {
    DrawFPS(8, 8);
    DrawText(m_mapManager->GetCurrentMapName().c_str(), 8, SCREEN_HEIGHT - 44, 16, YELLOW);
    DrawText("WASD / Arrow keys to move", 8, SCREEN_HEIGHT - 24, 16, LIGHTGRAY);
}