//
// Created by axel on 2026-05-10.
//

#ifndef POKEMONRPG_MAPMANAGER_H
#define POKEMONRPG_MAPMANAGER_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "TileMap.h"
#include "NPC.h"

struct DoorLink {
    int tileX, tileY; // dörrtilens position
    std::string targetMap; // vilken karta den leder till
    float spawnX, spawnY; // spelarens startpos i målkartan (world coords)
};

class MapManager {
public:
    MapManager();

    TileMap* GetCurrentMap() { return m_currentMap.get(); }
    std::string GetCurrentMapName() { return m_currentMapName; }

    void SwitchMap(const std::string& mapName, float spawnX, float spawnY);
    bool CheckDoorTrigger(float playerX, float playerY, float& outSpawnX, float& outSpawnY, std::string& outTargetMap);

    std::vector<NPC>& GetNPCs() { return m_npcs[m_currentMapName]; }

private:
    std::unique_ptr<TileMap> m_currentMap;
    std::string m_currentMapName;
    std::unordered_map<std::string, std::vector<NPC>> m_npcs;

    // Alla kartor
    std::unordered_map<std::string, std::vector<std::vector<int>>> m_mapData;
    // Dörrar per karta
    std::unordered_map<std::string, std::vector<DoorLink>> m_doors;

    void RegisterMaps();
};

#endif //POKEMONRPG_MAPMANAGER_H
