//
// Created by axel on 2026-05-10.
//

#ifndef POKEMONRPG_NPC_H
#define POKEMONRPG_NPC_H

#include <string>
#include "Constants.h"


class NPC {
public:
    NPC(float x, float y, const std::string& name, const std::string& dialog, SpriteConfig config = {});


    void Draw() const;
    bool IsNearPlayer(float playerX, float playerY) const;
    const std::string& GetDialog() const { return m_dialog;}
    const std::string& GetName() const { return m_name;}

private:
    float m_x, m_y;
    std::string m_name;
    std::string m_dialog;
    SpriteConfig m_config;
};


#endif //POKEMONRPG_NPC_H
