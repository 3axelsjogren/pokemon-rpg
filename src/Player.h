#pragma once
#include "raylib.h"
#include "TileMap.h"
#include "Constants.h"

enum class Direction { DOWN = 0, LEFT, RIGHT, UP };

class Player {
public:
    Player(float startX, float startY);

    void Update(float dt, const TileMap& map);
    void Draw() const;

    Vector2 GetPosition() const { return m_pos; }
    Vector2 GetCenter()   const { return { m_pos.x + TILE_SIZE / 2.0f,
                                           m_pos.y + TILE_SIZE / 2.0f }; }

private:
    Vector2   m_pos;
    Direction m_facing;
    bool      m_moving;

    // Animation
    float m_animTimer;
    int   m_animFrame;   // 0 or 1 (two-frame walk cycle)

    bool CanMoveTo(float nx, float ny, const TileMap& map) const;
    void DrawSprite() const;
};
