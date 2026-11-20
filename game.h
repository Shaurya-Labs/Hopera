#pragma once
#include <raylib.h>
#include "player.h"
#include <vector>
#include "common.h"

class Game {
    public:
        Player player{};
        Camera2D camera{};
        
        Game(bool debug);
        void init_player(float width, float height, Color color, int movement_speed);
        void init_camera(Vector2 offset, Vector2 player_initial_position, float rotation, float zoom);
        void get_input();
        void update(float dt);
        void draw(float alpha);
        
    private:
        std::vector<Wall> walls {
            Wall { 60, 500, 900, 500, BLUE },
            Wall { 60, 0, 60, 500, RED },
            Wall { 900, 0, 900, 500, GREEN }
        };
        
        void check_and_resolve_collisions();
};