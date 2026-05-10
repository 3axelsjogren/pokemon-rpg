#pragma once
#include "raylib.h"
#include "TileMap.hpp"
#include "Player.hpp"
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

    std::unique_ptr<TileMap> m_map;
    std::unique_ptr<Player>  m_player;
    Camera2D                 m_camera;
};
