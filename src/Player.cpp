#include "Player.h"
#include <cmath>

Player::Player(float startX, float startY) : m_pos({ startX, startY }), m_facing(Direction::DOWN), m_moving(false)
    , m_animTimer(0.0f), m_animFrame(0)
{}

void Player::Update(float dt, const TileMap& map) {
    Vector2 vel = { 0, 0 };

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) { vel.x =  PLAYER_SPEED; m_facing = Direction::RIGHT; }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) { vel.x = -PLAYER_SPEED; m_facing = Direction::LEFT;  }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) { vel.y =  PLAYER_SPEED; m_facing = Direction::DOWN;  }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) { vel.y = -PLAYER_SPEED; m_facing = Direction::UP;    }

    // Normalize diagonal movement
    float len = sqrtf(vel.x * vel.x + vel.y * vel.y);
    if (len > 0.0f) {
        vel.x = vel.x / len * PLAYER_SPEED;
        vel.y = vel.y / len * PLAYER_SPEED;
        m_moving = true;
    } else {
        m_moving = false;
    }

    // Collision - separate X and Y axes
    float newX = m_pos.x + vel.x * dt;
    float newY = m_pos.y + vel.y * dt;

    // Check X movement (test all four corners)
    if (CanMoveTo(newX, m_pos.y, map))
        m_pos.x = newX;

    // Check Y movement
    if (CanMoveTo(m_pos.x, newY, map))
        m_pos.y = newY;

    // Animation
    if (m_moving) {
        m_animTimer += dt;
        if (m_animTimer >= 0.15f) {
            m_animTimer = 0.0f;
            m_animFrame = (m_animFrame + 1) % 2;
        }
    } else {
        m_animFrame = 0;
        m_animTimer = 0.0f;
    }
}

bool Player::CanMoveTo(float nx, float ny, const TileMap& map) const {
    int margin = 2;
    int s = TILE_SIZE - margin;

    // Check all four corners of the player's bounding box
    return !map.IsSolidWorld(nx + margin,ny + margin)
        && !map.IsSolidWorld(nx + s,ny + margin)
        && !map.IsSolidWorld(nx + margin,ny + s)
        && !map.IsSolidWorld(nx + s,ny + s);
}

void Player::Draw() const {
    DrawSprite();
}

void Player::DrawSprite() const {
    int x = (int)m_pos.x;
    int y = (int)m_pos.y;
    int s = TILE_SIZE;

    // Body
    DrawRectangle(x + 6, y + 14, s - 12, s - 10, BLUE);

    // Head
    DrawRectangle(x + 8, y + 2, s - 16, 14, { 255, 213, 170, 255 });

    // Hair
    DrawRectangle(x + 8, y + 2, s - 16, 5, { 30, 30, 30, 255 });

    // Eyes (direction-dependent)
    Color eyeCol = BLACK;
    switch (m_facing) {
        case Direction::DOWN:
            DrawRectangle(x + 10, y + 8,  3, 3, eyeCol);
            DrawRectangle(x + 18, y + 8,  3, 3, eyeCol);
            break;
        case Direction::UP:
            // Back of head — no eyes visible
            break;
        case Direction::LEFT:
            DrawRectangle(x + 10, y + 8,  3, 3, eyeCol);
            break;
        case Direction::RIGHT:
            DrawRectangle(x + 18, y + 8,  3, 3, eyeCol);
            break;
    }

    // Legs (animated)
    int legOffset = m_moving ? (m_animFrame == 0 ? -2 : 2) : 0;
    DrawRectangle(x + 8,y + s - 10 + legOffset,6,10, DARKBLUE);
    DrawRectangle(x + s - 14,y + s - 10 - legOffset,6,10, DARKBLUE);
}
