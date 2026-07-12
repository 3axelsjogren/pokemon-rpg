#pragma once
#include "Constants.h"
#include "TileMap.h"
#include "raylib.h"

class Enemy {
public:
    Enemy(float x, float y, int hp, float speed, Color bodyColor);

    void Update(float dt, Vector2 playerPos, const TileMap& map);
    void Draw() const;

    bool IsAlive() const {return m_hp > 0;};
    int GetHP() const { return m_hp; }
    int GetMaxHP() const { return m_maxHp; }
    void TakeDamage(int amount) { m_hp -= amount; if (m_hp < 0) m_hp = 0; }
    Vector2 GetPosition() const { return m_pos; }
    bool CollidesWithPlayer(Vector2 playerPos) const;
    bool IsInAttackRange(Vector2 playerPos) const;

private:
    Vector2 m_pos;
    int m_hp;
    int m_maxHp;
    float m_speed;
    Color m_bodyColor;
};