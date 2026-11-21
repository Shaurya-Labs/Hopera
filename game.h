#pragma once
#include <raylib.h>
#include "player.h"
#include "map.h"

class Game {
    public:
        Player player{};
        Camera2D camera{};
        Map map{};
        
        Game(bool debug);
        void init_player(float width, float height, Color color, int movement_speed);
        void init_camera(Vector2 offset, Vector2 player_initial_position, float rotation, float zoom);
        void get_input();
        void update(float dt);
        void draw(float alpha);
        void unload();      // Unload all textures and audio from all the objects
        
    private:
        void check_and_resolve_collisions();
};