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
    { 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2 },
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

// Utomhuskartan
static const std::vector<std::vector<int>> FIRST_FILLER_MAP = { // TODO: rita om nytt landksap med bro och flod, bara filler
    { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 2, 2, 0, 0, 0, 4, 4, 4, 4, 0, 0, 2, 2, 0, 0, 0, 2 },
    { 2, 0, 0, 2, 2, 0, 0, 0, 4, 4, 6, 4, 0, 0, 2, 2, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 2 },
    { 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2 },
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

MapManager::MapManager() {
    RegisterMaps();
    SwitchMap("outdoor", 10 * TILE_SIZE, 10 * TILE_SIZE);
}

void MapManager::RegisterMaps() {
    m_mapData["outdoor"] = OUTDOOR_MAP;
    m_mapData["house"] = HOUSE_MAP;
    m_mapData["woods_first"] = FIRST_FILLER_MAP; // TODO: utveckla stig till vänster för första scen-switchen till äventyret

    // Dörr på utomhuskartan: tile (10,4) leder till house
    m_doors["outdoor"].push_back({ 10, 4, "house", 3 * TILE_SIZE, 5 * TILE_SIZE });

    // Dörr i house: tile (3,6) leder tillbaka till outdoor
    m_doors["house"].push_back({ 3, 6, "outdoor", 10 * TILE_SIZE, 5 * TILE_SIZE });

    m_npcs["house"].push_back(NPC(3 * TILE_SIZE, 3 * TILE_SIZE, "Mamma",
    "Ah, du ar vaken! Ga ut och utforska varlden, mitt barn.", { RED, { 180, 100, 20, 255 }, { 255, 213, 170, 255 } }));
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