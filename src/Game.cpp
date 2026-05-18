#include "Game.h"

#include <cfloat>
#include <cmath>

#include "Constants.h"

Game::Game() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    InitAudioDevice();
    m_musicOverworld = LoadMusicStream("assets/music/main_theme.ogg");
    m_musicCave = LoadMusicStream("assets/music/cave.ogg");
    SetMusicVolume(m_musicOverworld, 0.1f);  // 0.0 = tyst, 1.0 = max
    SetMusicVolume(m_musicCave, 0.1f);
    m_musicCurrent = m_musicOverworld;
    PlayMusicStream(m_musicCurrent);

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
    UnloadMusicStream(m_musicOverworld);
    UnloadMusicStream(m_musicCave);
    CloseAudioDevice();
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
        if (targetMap == "blockage" && m_state.hasGem)
            targetMap = "blockage_unlocked";
        m_mapManager->SwitchMap(targetMap);
        m_player->SetPosition(spawnX, spawnY);
        m_camera.target = m_player->GetCenter();
    }

    // Musikbyte
    std::string mapName = m_mapManager->GetCurrentMapName();
    Music newMusic = (mapName == "q1_cave" || mapName == "q1_cave_no_gem")
        ? m_musicCave : m_musicOverworld;

    if (newMusic.stream.buffer != m_musicCurrent.stream.buffer) {
        StopMusicStream(m_musicCurrent);
        m_musicCurrent = newMusic;
        PlayMusicStream(m_musicCurrent);
    }

    UpdateMusicStream(m_musicCurrent);

    // Plocka upp kristall
    if (!m_state.hasGem) {
        int tileX = static_cast<int>(center.x / TILE_SIZE);
        int tileY = static_cast<int>(center.y / TILE_SIZE);
        if (m_mapManager->GetCurrentMapName() == "q1_cave" && tileX == 16 && tileY == 2) {
            m_state.hasGem = true;
            m_mapManager->SwitchMap("q1_cave_no_gem");
            m_dialog.Show("!", "You found a golden crystal! The guards fear your courage...");
        }
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
            NPC* closest = nullptr;
            float closestDist = FLT_MAX;

            for (auto& npc : m_mapManager->GetNPCs()) {
                float dx = pos.x - npc.GetX();
                float dy = pos.y - npc.GetY();
                float dist = sqrtf(dx*dx + dy*dy);
                if (dist < TILE_SIZE * 2.0f && dist < closestDist) {
                    closestDist = dist;
                    closest = &npc;
                }
            }
            if (closest)
                m_dialog.Show(closest->GetName(), closest->GetDialog());
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