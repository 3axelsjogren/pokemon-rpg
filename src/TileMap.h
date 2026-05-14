#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "Constants.h"

// Tile IDs
enum TileType {
    // outside/houses
    TILE_GRASS = 0,
    TILE_WATER = 1,
    TILE_TREE = 2,
    TILE_PATH = 3,
    TILE_HOUSE = 4,
    TILE_BRIDGE = 5,
    TILE_DOOR = 6,
    TILE_FLOOR = 7,
    TILE_LOCKED_DOOR = 8,
    TILE_LEVER = 9, // framtida användning

    // Caves
    TILE_CAVE_WALL = 10,
    TILE_CAVE_FLOOR = 11,
    TILE_GEM = 12,
};

struct Tile {
    TileType type;
    bool solid;  // blocks movement
};

class TileMap {
public:
    TileMap(int width, int height);
    ~TileMap();

    void LoadFromArray(const std::vector<std::vector<int>>& data);
    void Draw(Camera2D camera) const;

    bool IsSolid(int tileX, int tileY) const;
    bool IsSolidWorld(float worldX, float worldY) const;

    int GetWidth()  const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    int m_width, m_height;
    std::vector<std::vector<Tile>> m_tiles;

    Color TileColor(TileType type) const;
};
