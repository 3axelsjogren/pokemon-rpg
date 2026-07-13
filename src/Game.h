#pragma once

#include "raylib.h"
#include "MapManager.h"
#include "Player.h"
#include "DialogBox.h"
#include "Constants.h"
#include "BattleManager.h"
#include <memory>

class Game {
public:
    Game();
    ~Game();
    void Run();

private:
    void Update(float dt);
    void Draw();
    void DrawHUD();

    std::unique_ptr<MapManager> m_mapManager;
    std::unique_ptr<Player> m_player;
    Camera2D m_camera;

    DialogBox m_dialog;

    GameState m_state;

    BattleManager m_battle;
    bool m_inBossRoom = false;

    //musik
    Music m_musicOverworld;
    Music m_musicCave;
    Music m_musicCity;
    Music m_musicPuzzle1;
    Music m_musicCorridor;
    Music m_musicBoss1;
    Music m_musicVictory;

    Music m_musicCurrent;

    //sfx
    Sound m_sfxAttack;
    Sound m_sfxHit;
    Sound m_sfxDoor;
    Sound m_sfxPickup;
};
