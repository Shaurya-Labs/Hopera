#include "map.h"
#include <raylib.h>

Map::Map() {
    map_texture_tile = LoadTexture("assets/graphics/map/map.png");

    normal_map_rect = { 0.0f, 0.0f, static_cast<float>(map_texture_tile.width), static_cast<float>(map_texture_tile.height) };
    mirrored_map_rect = { 0.0f, 0.0f, -static_cast<float>(map_texture_tile.width), static_cast<float>(map_texture_tile.height) };
}

void Map::unload() {
    // Unload all map textures
    UnloadTexture(map_texture_tile);
}

void Map::draw() {
    float scale = 800.0f / static_cast<float>(map_texture_tile.height);
    float draw_width = static_cast<float>(map_texture_tile.width) * scale;
    float draw_height = 800.0f;

    DrawTexturePro(map_texture_tile, mirrored_map_rect, Rectangle{ 0.0f, 0.0f, draw_width, draw_height }, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(map_texture_tile, normal_map_rect, Rectangle{ draw_width, 0.0f, draw_width, draw_height }, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(map_texture_tile, mirrored_map_rect, Rectangle{ draw_width*2, 0.0f, draw_width, draw_height }, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(map_texture_tile, normal_map_rect, Rectangle{ draw_width*3, 0.0f, draw_width, draw_height }, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}