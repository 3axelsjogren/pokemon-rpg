//
// Created by axel on 2026-05-10.
//

#include "MapManager.h"
#include "Constants.h"

// Utomhuskartan
static const std::vector<std::vector<int>> OUTDOOR_MAP = {
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 2, 2, 0, 0, 0, 4, 4, 4, 4, 0, 0, 2, 2, 0, 0, 0, 2 },
    { 2, 0, 0, 2, 2, 0, 0, 0, 4, 4, 6, 4, 0, 0, 2, 2, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 6, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 1, 1, 1, 1, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2 },
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
};

// Inomhuskartan (litet rum)
static const std::vector<std::vector<int>> HOUSE_MAP = {
    { 4, 4, 4, 4, 4, 4, 4, 4 },
    { 4, 7, 7, 7, 7, 7, 7, 4 },
    { 4, 7, 7, 7, 7, 7, 7, 4 },
    { 4, 7, 7, 7, 7, 7, 7, 4 },
    { 4, 7, 7, 7, 7, 7, 7, 4 },
    { 4, 7, 7, 7, 7, 7, 7, 4 },
    { 4, 4, 4, 6, 4, 4, 4, 4 },  // 6 = dörr ut, 4 = väggar, 7 = golv
};

// Äventyr-start
static const std::vector<std::vector<int>> FIRST_FILLER_MAP = {
    { 2, 2, 6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 2, 0, 3, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 3, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2 },
    { 2, 0, 3, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 2 },
    { 2, 0, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2 },
    { 2, 0, 3, 0, 0, 0, 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 3, 0, 0, 0, 0, 1, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 3, 3, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 0, 0, 3, 5, 5, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 3, 3, 5, 5, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 2 }, // stig -> bro på denna rad
    { 2, 0, 0, 0, 0, 0, 3, 5, 5, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2 },
    { 2, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2 },
    { 2, 0, 1, 1, 1, 0, 0, 1, 1, 0, 2, 2, 0, 0, 3, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 2, 2, 0, 3, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 6 },
    { 2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2 },
    { 2, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2 },
    { 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
};

// Blockage-interaction, story intro
static const std::vector<std::vector<int>> BLOCKAGE_MAP = {
    { 2, 2, 2, 6, 6, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 2, 0, 0, 2 },
    { 2, 2, 2, 8, 8, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2 },
    { 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2 },
    { 2, 2, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 2, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 2, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 2, 0, 0, 1, 1, 1, 0, 0, 0, 2 },
    { 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 2 },
    { 2, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1 },
    { 2, 0, 3, 0, 0, 0, 0, 0, 2, 2, 1, 1, 0, 0, 2, 0, 0, 0, 1, 1 },
    { 2, 0, 3, 0, 0, 0, 0, 2, 2, 2, 1, 1, 0, 2, 2, 2, 2, 2, 0, 2 },
    { 2, 2, 6, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
};

MapManager::MapManager() {
    RegisterMaps();
    SwitchMap("outdoor", 10 * TILE_SIZE, 10 * TILE_SIZE);
}

void MapManager::RegisterMaps() {
    m_mapData["outdoor"] = OUTDOOR_MAP;
    m_mapData["house"] = HOUSE_MAP;
    m_mapData["woods_first"] = FIRST_FILLER_MAP;
    m_mapData["blockage"] = BLOCKAGE_MAP;

    // Dörr på utomhuskartan: tile (10,4) leder till house
    m_doors["outdoor"].push_back({ 10, 4, "house", 3 * TILE_SIZE, 5 * TILE_SIZE });
    // Dörr i house: tile (3,6) leder tillbaka till outdoor
    m_doors["house"].push_back({ 3, 6, "outdoor", 10 * TILE_SIZE, 5 * TILE_SIZE });
    m_npcs["house"].push_back(NPC(3 * TILE_SIZE, 3 * TILE_SIZE, "Mamma",
    "Ah you're awake! Go out and explore the world, my child!", { RED, { 180, 100, 20, 255 }, { 255, 213, 170, 255 } }));

    // Trigger på outdoor: tile (0,6) - stigen vid vänsterkanten
    m_doors["outdoor"].push_back({ 0, 6, "woods_first", 18 * TILE_SIZE, 15 * TILE_SIZE });
    // Trigger tillbaka: tile (19,15) på first_filler
    m_doors["woods_first"].push_back({ 19, 15, "outdoor", 1 * TILE_SIZE, 6 * TILE_SIZE });

    // dörr till blockage
    m_doors["woods_first"].push_back({ 2, 0, "blockage", 2 * TILE_SIZE, 18 * TILE_SIZE });
    // dörr tillbaka
    m_doors["blockage"].push_back({ 2, 19, "woods_first", 2 * TILE_SIZE, 1 * TILE_SIZE });
}

void MapManager::SwitchMap(const std::string& mapName, float spawnX, float spawnY) {
    auto& data = m_mapData[mapName];
    int w = static_cast<int>(data[0].size());
    int h = static_cast<int>(data.size());
    m_currentMap = std::make_unique<TileMap>(w, h);
    m_currentMap->LoadFromArray(data);
    m_currentMapName = mapName;
}

bool MapManager::CheckDoorTrigger(float playerX, float playerY, float& outSpawnX, float& outSpawnY, std::string& outTargetMap) {
    int tileX = static_cast<int>(playerX / TILE_SIZE);
    int tileY = static_cast<int>(playerY / TILE_SIZE);

    for (auto& door : m_doors[m_currentMapName]) {
        if (door.tileX == tileX && door.tileY == tileY) {
            outSpawnX = door.spawnX;
            outSpawnY = door.spawnY;
            outTargetMap = door.targetMap;
            return true;
        }
    }
    return false;
}