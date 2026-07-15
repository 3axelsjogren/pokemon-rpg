#include "Game.h"

#include <cfloat>
#include <cmath>

#include "Constants.h"

Game::Game(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    InitAudioDevice();

    //musik
    m_musicOverworld = LoadMusicStream("assets/music/main_theme.ogg");
    SetMusicVolume(m_musicOverworld, 0.1f); // 0.0 = tyst, 1.0 = max

    m_musicCave = LoadMusicStream("assets/music/cave.ogg");
    SetMusicVolume(m_musicCave, 0.1f);

    m_musicCity = LoadMusicStream("assets/music/city1.ogg");
    SetMusicVolume(m_musicCity, 0.1f);

    m_musicPuzzle1 = LoadMusicStream("assets/music/puzzle1.ogg");
    SetMusicVolume(m_musicPuzzle1, 0.1f);

    m_musicCorridor = LoadMusicStream("assets/music/corridor.ogg");
    SetMusicVolume(m_musicCorridor, 0.5f);

    m_musicBoss1 = LoadMusicStream("assets/music/boss1.ogg");
    SetMusicVolume(m_musicBoss1, 0.4f);

    m_musicVictory = LoadMusicStream("assets/music/victory.ogg");
    SetMusicVolume(m_musicVictory, 0.4f);

    m_musicCurrent = m_musicOverworld;
    PlayMusicStream(m_musicCurrent);

    // sfx
    m_sfxAttack = LoadSound("assets/sfx/attack.ogg");
    m_sfxHit = LoadSound("assets/sfx/hit.ogg");
    m_sfxDoor = LoadSound("assets/sfx/door.ogg");
    SetSoundVolume(m_sfxDoor, 0.4f);
    m_sfxPickup = LoadSound("assets/sfx/pickup.ogg");
    SetSoundVolume(m_sfxPickup, 0.4f);

    m_mapManager = std::make_unique<MapManager>();

    m_player = std::make_unique<Player>(6 * TILE_SIZE, 8 * TILE_SIZE); // spawn

    m_camera = {};
    m_camera.zoom = 1.5f;
    m_camera.offset = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    m_camera.target = m_player->GetCenter();
    m_camera.rotation = 0.0f;
}

Game::~Game(){
    CloseWindow();
    UnloadMusicStream(m_musicOverworld);
    UnloadMusicStream(m_musicCave);
    UnloadMusicStream(m_musicCity);
    UnloadMusicStream(m_musicPuzzle1);
    UnloadMusicStream(m_musicCorridor);
    UnloadMusicStream(m_musicBoss1);
    UnloadMusicStream(m_musicVictory);

    UnloadSound(m_sfxAttack);
    UnloadSound(m_sfxHit);
    UnloadSound(m_sfxDoor);
    UnloadSound(m_sfxPickup);
    CloseAudioDevice();
}

void Game::Run(){
    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        Update(dt);
        Draw();
    }
}

void Game::Update(float dt){
    TileMap *map = m_mapManager->GetCurrentMap();
    m_player->Update(dt, *map);

    if (m_mapManager->GetCurrentMapName() == "temple_puzzle"){
        if (!m_state.puzzleActive){
            m_state.puzzleActive = true;
            m_state.puzzleTimer = 30.0f;
        }
        m_state.puzzleTimer -= dt;

        if (m_state.puzzleTimer <= 0.0f){
            m_state.puzzleActive = false;
            m_mapManager->SwitchMap("temple_out");
            m_player->SetPosition(9 * TILE_SIZE, 18 * TILE_SIZE);
            m_camera.target = m_player->GetCenter();
            m_dialog.Show("!", "Time's up! Try again.");
        }

        Vector2 pos = m_player->GetPosition();
        int tileX = static_cast<int>((pos.x + TILE_SIZE / 2) / TILE_SIZE);
        int tileY = static_cast<int>((pos.y + TILE_SIZE / 2) / TILE_SIZE);
        TileType tile = map->GetTileType(tileX, tileY);

        if (tile != TILE_CAVE_FLOOR && tile != TILE_DOOR && tile != TILE_CAVE_WALL){
            m_state.puzzleTimer = 30.0f;
            m_player->SetPosition(4 * TILE_SIZE, 17 * TILE_SIZE);
        }
    }

    // Kolla dörrtrigger
    float spawnX, spawnY;
    std::string targetMap;
    Vector2 center = m_player->GetCenter();
    if (m_mapManager->CheckDoorTrigger(center.x, center.y, spawnX, spawnY, targetMap)){
        PlaySound(m_sfxDoor);
        if (targetMap == "blockage" && m_state.hasGem) targetMap = "blockage_unlocked";

        //m_state.hasGem = true; // tillfällig under utveckling --- MISSA INTE
        if (targetMap == "temple_puzzle" && !m_state.hasGem){
            m_dialog.Show("!", "The temple is sealed. You need the crystal.");
        } else {
            if (targetMap != "temple_puzzle") m_state.puzzleActive = false;
            m_mapManager->SwitchMap(targetMap);
            m_player->SetPosition(spawnX, spawnY);
            m_camera.target = m_player->GetCenter();
        }
    }

    // Boss-rum
    if (m_mapManager->GetCurrentMapName() == "boss_room"){
        if (!m_inBossRoom && !m_state.bossDefeated){
            m_inBossRoom = true;
            m_battle.SpawnBoss(10 * TILE_SIZE, 9 * TILE_SIZE);
        }

        if (!m_state.bossDefeated){
            bool attack = IsKeyPressed(KEY_SPACE);
            if (m_battle.AttackLanded()){
                PlaySound(m_sfxAttack);
                m_battle.ResetAttackFlag();
            }
            m_battle.Update(dt, m_player->GetPosition(), attack, *map);

            if (m_battle.PickupCollected()){
                PlaySound(m_sfxPickup);
                m_battle.ResetPickupFlag();
            }

            if (m_battle.PlayerTookDamage()){
                PlaySound(m_sfxHit);
                m_battle.ResetDamageFlag();
            }

            if (!m_battle.IsBossAlive()){
                m_inBossRoom = false;
                m_state.bossDefeated = true;
                m_state.hasGem2 = true;
                m_dialog.Show("!", "The ancient guardian falls... A second crystal appears!");
            }

            if (m_battle.GetPlayerHP() <= 0){
                m_inBossRoom = false;
                m_battle.Reset();
                m_mapManager->SwitchMap("corridor_1");
                m_player->SetPosition(7 * TILE_SIZE, 19 * TILE_SIZE);
                m_camera.target = m_player->GetCenter();
                m_dialog.Show("!", "You were defeated... Try again.");
            }
        }
    } else {
        m_inBossRoom = false;
    }

    // Musikbyte
    std::string mapName = m_mapManager->GetCurrentMapName();
    Music newMusic;
    if (mapName == "q1_cave" || mapName == "q1_cave_no_gem")
        newMusic = m_musicCave;
    else if (mapName == "temple_puzzle")
        newMusic = m_musicPuzzle1;
    else if (mapName == "corridor_1")
        newMusic = m_musicCorridor;
    else if (mapName == "boss_room")
        newMusic = m_battle.IsBossAlive() ? m_musicBoss1 : m_musicVictory;
    else if (mapName == "first_city" || mapName == "apartament_1" ||
             mapName == "apartament_2" || mapName == "apartament_3" ||
             mapName == "apartament_4" || mapName == "apartament_5")
        newMusic = m_musicCity;
    else
        newMusic = m_musicOverworld;

    if (newMusic.stream.buffer != m_musicCurrent.stream.buffer){
        StopMusicStream(m_musicCurrent);
        m_musicCurrent = newMusic;
        PlayMusicStream(m_musicCurrent);
    }
    UpdateMusicStream(m_musicCurrent);

    // Plocka upp kristall
    if (!m_state.hasGem){
        int tileX = static_cast<int>(center.x / TILE_SIZE);
        int tileY = static_cast<int>(center.y / TILE_SIZE);
        if (m_mapManager->GetCurrentMapName() == "q1_cave" && tileX == 16 && tileY == 2){
            m_state.hasGem = true;
            m_mapManager->SwitchMap("q1_cave_no_gem");
            m_dialog.Show("!", "You found a golden crystal! The guards fear your courage...");
        }
    }

    // Smooth camera
    center = m_player->GetCenter();
    m_camera.target.x += (center.x - m_camera.target.x) * 8.0f * dt;
    m_camera.target.y += (center.y - m_camera.target.y) * 8.0f * dt;

    // Kamera-begränsning
    TileMap* currentMap = m_mapManager->GetCurrentMap();
    float mapW = currentMap->GetWidth() * TILE_SIZE;
    float mapH = currentMap->GetHeight() * TILE_SIZE;

    float halfW = (SCREEN_WIDTH / 2.0f) / m_camera.zoom;
    float halfH = (SCREEN_HEIGHT / 2.0f) / m_camera.zoom;

    if (m_camera.target.x < halfW) m_camera.target.x = halfW;
    if (m_camera.target.y < halfH) m_camera.target.y = halfH;
    if (m_camera.target.x > mapW - halfW) m_camera.target.x = mapW - halfW;
    if (m_camera.target.y > mapH - halfH) m_camera.target.y = mapH - halfH;

    // NPC-interaktion
    if (IsKeyPressed(KEY_E)){
        if (m_dialog.IsOpen()){
            m_dialog.Close();
        } else {
            Vector2 pos = m_player->GetPosition();
            NPC *closest = nullptr;
            float closestDist = FLT_MAX;

            for (auto &npc : m_mapManager->GetNPCs()){
                float dx = pos.x - npc.GetX();
                float dy = pos.y - npc.GetY();
                float dist = sqrtf(dx * dx + dy * dy);
                if (dist < TILE_SIZE * 2.0f && dist < closestDist){
                    closestDist = dist;
                    closest = &npc;
                }
            }
            if (closest) m_dialog.Show(closest->GetName(), closest->GetDialog());
        }
    }
}

void Game::Draw(){
    BeginDrawing();
    ClearBackground({20, 20, 20, 255});
    BeginMode2D(m_camera);
    m_mapManager->GetCurrentMap()->Draw(m_camera);
    for (auto &npc : m_mapManager->GetNPCs())
        npc.Draw();
    if (m_mapManager->GetCurrentMapName() == "boss_room")
        m_battle.Draw();
    m_player->Draw();
    EndMode2D();
    if (m_mapManager->GetCurrentMapName() == "boss_room")
        m_battle.DrawHUD();
    m_dialog.Draw();
    DrawHUD();
    EndDrawing();
}

void Game::DrawHUD(){
    DrawText(TextFormat("FPS: %d", GetFPS()), 8, 8, 16, YELLOW);
    DrawText(m_mapManager->GetCurrentMapName().c_str(), 8, SCREEN_HEIGHT - 44, 16, YELLOW);
    DrawText("WASD / Arrow keys to move", 8, SCREEN_HEIGHT - 24, 16, LIGHTGRAY);
    if (m_mapManager->GetCurrentMapName() == "temple_puzzle"){
        std::string timerText = "Time: " + std::to_string((int)m_state.puzzleTimer);
        DrawText(timerText.c_str(), SCREEN_WIDTH / 2 - 40, 10, 24, YELLOW);
    }

    if (m_mapManager->GetCurrentMapName() == "boss_room") {
        DrawText("SPACE - Attack", SCREEN_WIDTH - 150, SCREEN_HEIGHT - 24, 16, LIGHTGRAY);
    }
}