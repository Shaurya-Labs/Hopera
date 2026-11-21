#pragma once
#include <raylib.h>
#include <vector>
#include "common.h"

class Map {
    public:
        Texture2D map_texture_tile{};
        Texture2D npc_sprite_finding{};
        Texture2D npc_spritesheet_happy{};
        Texture2D bike_sprite{};
        Texture2D trash_bin_sprite{};
        std::vector<Wall> walls {
            Wall { 60, 582, 5000, 582, BLUE },      // Bottom
            Wall { 60, 0, 60, 640, RED },       // Left
            Wall { 5000, 0, 5000, 640, GREEN }      // Right
        };

        Rectangle npc { 182, 510, 45, 60 };

        Map();
        void unload();      // Unloads all textures and audio
        void check_and_resolve_collisions(Camera2D camera);
        void update(float dt);
        void draw();
        void draw_static_elements();        // Should be placed after EndMode2D() in game.cpp draw() function
        
        private:
        Rectangle normal_map_rect{};
        Rectangle mirrored_map_rect{};
        bool show_npc_start_dialogue{ false };
        float subtitle_timer{};

        std::vector<Rectangle> bikes {
            Rectangle { 460.0f,  532.0f, 80.0f, 50.0f },
            Rectangle { 1120.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 1540.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 2380.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 3190.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 4120.0f, 532.0f, 80.0f, 50.0f }
        };

        std::vector<Rectangle> trash_bins {
            Rectangle { 760.0f,  536.0f, 48.0f, 46.0f },
            Rectangle { 1890.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 2110.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 2760.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 3510.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 3740.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 4560.0f, 536.0f, 48.0f, 46.0f }
        };
        
        void draw_subtitle(const char* text, int font_size = 24);
        void draw_sprite(const Texture2D& texture, const Rectangle& hitbox, float target_height, Color tint = WHITE);
};