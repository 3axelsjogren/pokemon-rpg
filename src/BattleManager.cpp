#include "BattleManager.h"
#include "Constants.h"

BattleManager::BattleManager()
    : m_playerHp(100), m_playerMaxHp(100),
      m_playerTookDamage(false), m_damageCooldown(0.0f) {}

void BattleManager::SpawnBoss(float x, float y) {
    m_enemies.clear();
    m_enemies.push_back(Enemy(x, y, 300, 45.0f, { 80, 20, 20, 255 })); // hp, första parameter, speed, andra parameter 
}

void BattleManager::Update(float dt, Vector2 playerPos, bool playerAttacks, const TileMap& map) {

    m_damageCooldown -= dt;
    m_playerAttackCooldown -= dt;

    // Spawna health pickup var 10:e sekund
    m_pickupSpawnTimer -= dt;
    if (m_pickupSpawnTimer <= 0.0f) {
        m_pickupSpawnTimer = 10.0f;
        // Spawna på slumpmässig position i rummet
        float rx = (float)(3 + rand() % 14) * TILE_SIZE;
        float ry = (float)(3 + rand() % 14) * TILE_SIZE;
        m_pickups.push_back({ {rx, ry}, true });
    }

    // Kolla om spelaren plockar upp
    for (auto& pickup : m_pickups) {
        if (!pickup.active) continue;
        float dx = playerPos.x - pickup.pos.x;
        float dy = playerPos.y - pickup.pos.y;
        if (sqrtf(dx*dx + dy*dy) < TILE_SIZE) {
            pickup.active = false;
            m_playerHp = std::min(m_playerHp + 30, m_playerMaxHp);
            m_pickupCollected = true;
        }
    }

    for (auto& enemy : m_enemies) {
        enemy.Update(dt, playerPos, map);

        // Boss skadar spelaren
        if (enemy.IsAlive() && enemy.CollidesWithPlayer(playerPos) && m_damageCooldown <= 0.0f) {
            m_playerHp -= 20;
            m_playerTookDamage = true;
            m_damageCooldown = 2.0f; // 2 sekunders cooldown
            if (m_playerHp < 0) m_playerHp = 0;
        }

        // Spelaren attackerar bossen
        if ((playerAttacks && m_playerAttackCooldown <= 0.0f && enemy.IsAlive() && enemy.IsInAttackRange(playerPos))) {
            enemy.TakeDamage(10);
            m_playerAttackCooldown = 1.0f;
            m_attackLanded = true;
        }
    }
}

void BattleManager::Draw() const {
    for (auto& enemy : m_enemies)
        enemy.Draw();
    
    for (auto& pickup : m_pickups) {
    if (!pickup.active) continue;
    DrawRectangle((int)pickup.pos.x, (int)pickup.pos.y, TILE_SIZE/2, TILE_SIZE/2, GREEN);
    DrawRectangle((int)pickup.pos.x + TILE_SIZE/8, (int)pickup.pos.y - TILE_SIZE/8, TILE_SIZE/4, TILE_SIZE/4, GREEN);
}
}

void BattleManager::DrawHUD() const {
    if (m_enemies.empty()) return;

    // Boss HP-bar
    const auto& boss = m_enemies[0];
    float pct = (float)boss.GetHP() / boss.GetMaxHP();
    DrawRectangle(200, 20, 400, 20, DARKGRAY);
    DrawRectangle(200, 20, (400 * pct), 20, RED);
    DrawRectangleLines(200, 20, 400, 20, WHITE);
    DrawText("BOSS", 160, 20, 18, WHITE);

    // Spelare HP-bar
    float playerPct = (float)m_playerHp / m_playerMaxHp;
    DrawRectangle(10, 40, 150, 16, DARKGRAY);
    DrawRectangle(10, 40, (150 * playerPct), 16, GREEN);
    DrawRectangleLines(10, 40, 150, 16, WHITE);
    DrawText("HP", 10, 24, 16, WHITE);
}

bool BattleManager::IsBossAlive() const {
    for (auto& e : m_enemies)
        if (e.IsAlive()) return true;
    return false;
}

void BattleManager::Reset() {
    m_enemies.clear();
    m_playerHp = m_playerMaxHp;
    m_playerTookDamage = false;
    m_damageCooldown = 0.0f;
    m_playerAttackCooldown = 0.0f;
    m_attackLanded = false;
    m_pickupCollected = false;
    
    m_pickups.clear();
    m_pickupSpawnTimer = 0.0f;
}