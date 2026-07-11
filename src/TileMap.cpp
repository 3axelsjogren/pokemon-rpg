#include "TileMap.h"

TileMap::TileMap(int width, int height) : m_width(width), m_height(height) {
    m_tiles.resize(height, std::vector<Tile>(width, { TILE_GRASS, false }));
}

TileMap::~TileMap() {}

void TileMap::LoadFromArray(const std::vector<std::vector<int>>& data) {
    for (int y = 0; y < static_cast<int>(data.size()) && y < m_height; ++y) {
        for (int x = 0; x < static_cast<int>(data[y].size()) && x < m_width; ++x) {
            TileType type = static_cast<TileType>(data[y][x]);
            bool solid = (type == TILE_WATER || type == TILE_TREE || type == TILE_HOUSE
                || type == TILE_LOCKED_DOOR || type == TILE_CAVE_WALL || type == TILE_VOID); // addera AND på nya solids
            m_tiles[y][x] = { type, solid };
        }
    }
}

void TileMap::Draw(Camera2D camera) const {
    // Only draw tiles visible on screen (culling)
    int startX = static_cast<int>(camera.target.x - camera.offset.x) / TILE_SIZE - 1;
    int startY = static_cast<int>(camera.target.y - camera.offset.y) / TILE_SIZE - 1;
    int endX = startX + SCREEN_WIDTH / TILE_SIZE + 3;
    int endY = startY + SCREEN_HEIGHT / TILE_SIZE + 3;

    startX = (startX < 0) ? 0 : startX;
    startY = (startY < 0) ? 0 : startY;
    endX = (endX > m_width) ? m_width : endX;
    endY = (endY > m_height) ? m_height : endY;

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            Color c = TileColor(m_tiles[y][x].type);
            DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, c);

            // Subtle grid
            DrawRectangleLines(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, ColorAlpha(BLACK, 0.08f));
        }
    }
}

bool TileMap::IsSolid(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= m_width || tileY >= m_height) {
        return true;  // out of bounds = solid
    }
    return m_tiles[tileY][tileX].solid;
}

bool TileMap::IsSolidWorld(float worldX, float worldY) const {
    const int tx = static_cast<int>(worldX / TILE_SIZE);
    const int ty = static_cast<int>(worldY / TILE_SIZE);
    return IsSolid(tx,ty);
}

TileType TileMap::GetTileType(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= m_width || tileY >= m_height)
        return TILE_VOID;
    return m_tiles[tileY][tileX].type;
}

Color TileMap::TileColor(TileType type) const {
    switch (type) {
        case TILE_GRASS: return { 106, 168, 79,  255 };
        case TILE_WATER: return { 70,  130, 180, 255 };
        case TILE_TREE: return { 39,  78,  19,  255 };
        case TILE_PATH: return { 194, 160, 109, 255 };
        case TILE_HOUSE: return { 180, 100, 60,  255 };
        case TILE_BRIDGE: return { 160, 120, 80,  255 };
        case TILE_DOOR: return { 120, 60,  20,  255 };
        case TILE_FLOOR: return {220,200,100, 255};
        case TILE_LOCKED_DOOR: return{ 120, 60,  20,  235};
        case TILE_CAVE_WALL: return { 60,  60,  60,  255 };
        case TILE_CAVE_FLOOR: return { 120, 120, 120, 255 };
        case TILE_GEM: return { 220, 180, 50,  255 };
        case TILE_VOID: return {0, 0, 0, 0};
        case TILE_CITY_ROAD: return {100, 100, 110, 255};
        case TILE_LAVA: return {220, 80, 20, 255};
        default: return MAGENTA;
    }
}
