#pragma once
#include <raylib.h>
#include <vector>
#include "common.h"

class Map {
    public:
        Texture2D map_texture_tile{};
        Texture2D npc_sprite_finding{};
        Texture2D npc_sprite_happy{};
        Texture2D bike_sprite{};
        Texture2D trash_bin_sprite{};
        Texture2D bush_sprite{};
        Texture2D bag_sprite{};
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
        bool show_npc_game_won_dialogue{ false };
        float subtitle_timer{};
        bool bag_captured{ false };

        Rectangle reward_bag { 4900.0f, 542.0f, 42.0f, 40.0f };

        std::vector<Rectangle> bikes {
            Rectangle { 460.0f,  532.0f, 80.0f, 50.0f },
            Rectangle { 1260.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 2060.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 2860.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 3660.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 4460.0f, 532.0f, 80.0f, 50.0f }
        };

        std::vector<Rectangle> trash_bins {
            Rectangle { 760.0f,  536.0f, 48.0f, 46.0f },
            Rectangle { 1560.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 2360.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 3160.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 3960.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 4720.0f, 536.0f, 48.0f, 46.0f }
        };

        std::vector<Rectangle> bushes {
            Rectangle { 1010.0f, 526.0f, 74.0f, 56.0f },
            Rectangle { 1810.0f, 526.0f, 74.0f, 56.0f },
            Rectangle { 2610.0f, 526.0f, 74.0f, 56.0f },
            Rectangle { 3410.0f, 526.0f, 74.0f, 56.0f },
            Rectangle { 4210.0f, 526.0f, 74.0f, 56.0f }
        };
        
        void draw_subtitle(const char* text, int font_size = 24);
        void draw_sprite(const Texture2D& texture, const Rectangle& hitbox, float target_height, Color tint = WHITE);
};