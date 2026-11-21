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
        float timer{};
        bool timmer_running{ false };
        bool bag_captured{ false };
        Music bg_music{};
        Sound item_pickup{};
        Sound task_completed{};
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
        void update_music_stream();
        void draw();
        void draw_static_elements();        // Should be placed after EndMode2D() in game.cpp draw() function
        
    private:
        Rectangle normal_map_rect{};
        Rectangle mirrored_map_rect{};
        bool show_npc_start_dialogue{ false };
        bool show_npc_game_won_dialogue{ false };
        float subtitle_timer{};

        Rectangle reward_bag { 4900.0f, 542.0f, 42.0f, 40.0f };

        std::vector<Rectangle> bikes {
            Rectangle { 430.0f,  532.0f, 80.0f, 50.0f },
            Rectangle { 1180.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 1410.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 2290.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 3120.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 3890.0f, 532.0f, 80.0f, 50.0f },
            Rectangle { 4190.0f, 532.0f, 80.0f, 50.0f }
        };

        std::vector<Rectangle> trash_bins {
            Rectangle { 790.0f,  536.0f, 48.0f, 46.0f },
            Rectangle { 920.0f,  536.0f, 48.0f, 46.0f },
            Rectangle { 1870.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 2640.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 3470.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 3610.0f, 536.0f, 48.0f, 46.0f },
            Rectangle { 4580.0f, 536.0f, 48.0f, 46.0f }
        };

        std::vector<Rectangle> bushes {
            // Clump 1 (around x = 610–675)
            Rectangle { 610.0f,  526.0f, 74.0f, 56.0f },
            Rectangle { 645.0f,  532.0f, 74.0f, 56.0f },
            Rectangle { 678.0f,  523.0f, 74.0f, 56.0f },

            // Clump 2 (around x = 1630–1675)
            Rectangle { 1630.0f, 530.0f, 74.0f, 56.0f },
            Rectangle { 1668.0f, 524.0f, 74.0f, 56.0f },

            // Clump 3 (around x = 2040–2110)
            Rectangle { 2040.0f, 525.0f, 74.0f, 56.0f },
            Rectangle { 2075.0f, 533.0f, 74.0f, 56.0f },
            Rectangle { 2110.0f, 527.0f, 74.0f, 56.0f },

            // Clump 4 (around x = 2850–2895)
            Rectangle { 2850.0f, 529.0f, 74.0f, 56.0f },
            Rectangle { 2892.0f, 522.0f, 74.0f, 56.0f },

            // Clump 5 (around x = 4360–4430)
            Rectangle { 4360.0f, 526.0f, 74.0f, 56.0f },
            Rectangle { 4395.0f, 533.0f, 74.0f, 56.0f },
            Rectangle { 4428.0f, 524.0f, 74.0f, 56.0f }
        };
        
        void draw_dialogues(const char* text, int font_size = 24);
        void draw_sprite(const Texture2D& texture, const Rectangle& hitbox, float target_height, Color tint = WHITE);
};