#pragma once

#include "raylib.h"
#include "MapManager.h"
#include "Player.h"
#include "DialogBox.h"
#include "Constants.h"
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
};
