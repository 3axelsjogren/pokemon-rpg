#pragma once
#include "Enemy.h"
#include "raylib.h"
#include <vector>
#include <string>

struct HealthPickup {
    Vector2 pos;
    bool active;
};

class BattleManager {
public:
    BattleManager();

    void SpawnBoss(float x, float y);
    void Update(float dt, Vector2 playerPos, bool playerAttacks, const TileMap& map);
    void Draw() const;
    void DrawHUD() const;

    bool IsBossAlive() const;
    bool PlayerTookDamage() const { return m_playerTookDamage; }
    bool m_attackLanded = false;
    bool AttackLanded() const { return m_attackLanded; }
    void ResetAttackFlag() { m_attackLanded = false; }
    void ResetDamageFlag() { m_playerTookDamage = false; }
    bool m_pickupCollected = false;
    bool PickupCollected() const { return m_pickupCollected; }
    void ResetPickupFlag() { m_pickupCollected = false; }
    void Reset();

    int GetPlayerHP() const { return m_playerHp; }

private:
    std::vector<Enemy> m_enemies;
    int m_playerHp;
    int m_playerMaxHp;
    bool m_playerTookDamage;
    float m_damageCooldown;
    float m_playerAttackCooldown = 0.0f;

    //health
    std::vector<HealthPickup> m_pickups;
    float m_pickupSpawnTimer = 0.0f;
};