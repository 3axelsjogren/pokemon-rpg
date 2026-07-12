#pragma once
#include <raylib.h>

struct SpriteConfig {
    Color bodyColor = BLUE;
    Color hairColor = { 80, 40, 10, 255 };
    Color skinColor = { 255, 213, 170, 255 };
};

struct GameState {
    bool hasGem = false;
    bool hasGem2 = false;
    bool bossDefeated = false;
    float puzzleTimer = 0.0f;
    bool puzzleActive = false;
};

// Window
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int TARGET_FPS = 60;
constexpr const char* WINDOW_TITLE = "Pokemon RPG";

// Tiles
constexpr int TILE_SIZE = 32; // pixels per tile

// Player
constexpr float PLAYER_SPEED = 3.0f * TILE_SIZE; // pixels per second
