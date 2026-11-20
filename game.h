#pragma once
#include <raylib.h>
#include "player.h"

class Game {
    public:
        Player player{};
    
        void init_player(float width, float height, Color color, int movement_speed);
        void get_input();
        void update(float dt);
        void draw(float alpha);
};