#include "Enemy.h"
#include <cmath>

Enemy::Enemy(float x, float y, int hp, float speed, Color bodyColor)
    : m_pos({x, y}), m_hp(hp), m_maxHp(hp), m_speed(speed), m_bodyColor(bodyColor) {}

void Enemy::Update(float dt, Vector2 playerPos, const TileMap& map) {
    if (!IsAlive()) return;

    float dx = playerPos.x - m_pos.x;
    float dy = playerPos.y - m_pos.y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist > 0) {
        float nx = (dx / dist) * m_speed * dt;
        float ny = (dy / dist) * m_speed * dt;

        if (!map.IsSolidWorld(m_pos.x + nx + TILE_SIZE/2, m_pos.y + TILE_SIZE/2))
            m_pos.x += nx;
        if (!map.IsSolidWorld(m_pos.x + TILE_SIZE/2, m_pos.y + ny + TILE_SIZE/2))
            m_pos.y += ny;
    }
}

void Enemy::Draw() const {
    if (!IsAlive()) return;
    int x = (int)m_pos.x;
    int y = (int)m_pos.y;
    int s = TILE_SIZE;

    // Vänster arm
    DrawRectangle(x - s, y + s/2, s, s, m_bodyColor);
    DrawRectangleLinesEx({(float)(x - s), (float)(y + s/2), (float)s, (float)s}, 2, {200, 50, 0, 180});

    // Höger arm
    DrawRectangle(x + s, y + s/2, s, s, m_bodyColor);
    DrawRectangleLinesEx({(float)(x + s), (float)(y + s/2), (float)s, (float)s}, 2, {200, 50, 0, 180});

    // Huvud (mitten, lite större)
    DrawRectangle(x - s/4, y - s/4, (int)(s * 1.5f), (int)(s * 1.5f), m_bodyColor);

    // Inre kärna huvud
    DrawRectangle(x, y, s/2, s/2, {40, 10, 10, 255});

    // Ögon
    DrawRectangle(x, y + s/8, s/4, s/4, RED);
    DrawRectangle(x + s/4 + s/8, y + s/8, s/4, s/4, RED);

    // Pupiller
    DrawRectangle(x + s/16, y + s/8 + s/8, s/8, s/8, BLACK);
    DrawRectangle(x + s/4 + s/8 + s/16, y + s/8 + s/8, s/8, s/8, BLACK);

    // Mun
    DrawRectangle(x, y + s/2, (int)(s * 0.75f), s/8, {180, 0, 0, 255});

    // Tänder uppåt
    DrawRectangle(x + s/8, y + s/2 - s/6, s/8, s/6, WHITE);
    DrawRectangle(x + s/8 + s/4, y + s/2 - s/6, s/8, s/6, WHITE);
    DrawRectangle(x + s/8 + s/2, y + s/2 - s/6, s/8, s/6, WHITE);

    // Tänder nedåt
    DrawRectangle(x + s/4, y + s/2 + s/8, s/8, s/6, WHITE);
    DrawRectangle(x + s/4 + s/4, y + s/2 + s/8, s/8, s/6, WHITE);

    // Glöd runt hela
    DrawRectangleLinesEx({(float)(x - s/4), (float)(y - s/4), (float)(s * 1.5f), (float)(s * 1.5f)}, 3, {200, 50, 0, 180});

    // Klor på armarna
    DrawRectangle(x - s - s/4, y + s/2 + s/4, s/4, s/8, {200, 50, 0, 255});
    DrawRectangle(x - s - s/4, y + s/2 + s/2, s/4, s/8, {200, 50, 0, 255});
    DrawRectangle(x + s*2, y + s/2 + s/4, s/4, s/8, {200, 50, 0, 255});
    DrawRectangle(x + s*2, y + s/2 + s/2, s/4, s/8, {200, 50, 0, 255});
}

// Boss skadar spelaren — liten räckvidd
bool Enemy::CollidesWithPlayer(Vector2 playerPos) const {
    float dx = playerPos.x - m_pos.x;
    float dy = playerPos.y - m_pos.y;
    return sqrtf(dx * dx + dy * dy) < TILE_SIZE * 0.8f;
}

// Spelaren kan attackera bossen — lite större räckvidd
bool Enemy::IsInAttackRange(Vector2 playerPos) const {
    float dx = playerPos.x - m_pos.x;
    float dy = playerPos.y - m_pos.y;
    return sqrtf(dx * dx + dy * dy) < TILE_SIZE * 1.4f;
}