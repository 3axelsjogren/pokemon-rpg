//
// Created by axel on 2026-05-10.
//

#include "NPC.h"
#include "Constants.h"
#include <cmath>
#include <raylib.h>

NPC::NPC(float x, float y, const std::string& name, const std::string& dialog, SpriteConfig config)
    : m_x(x), m_y(y), m_name(name), m_dialog(dialog), m_config(config) {}

void NPC::Draw() const {
    int x = static_cast<int>(m_x);
    int y = static_cast<int>(m_y);
    int s = TILE_SIZE;

    DrawRectangle(x + 6, y + 14, s - 12, s - 10, m_config.bodyColor);
    DrawRectangle(x + 8, y + 2, s - 16, 14, m_config.skinColor);
    DrawRectangle(x + 8, y + 2, s - 16, 5, m_config.hairColor);

    // lägg till ögon (tittar neråt som default):
    DrawRectangle(x + 10, y + 8, 3, 3, BLACK);
    DrawRectangle(x + 18, y + 8, 3, 3, BLACK);
}

bool NPC::IsNearPlayer(float playerX, float playerY) const {
    float dx = playerX - m_x;
    float dy = playerY - m_y;
    float dist = sqrtf(dx*dx + dy*dy);
    return dist < TILE_SIZE * 2.0f;
}