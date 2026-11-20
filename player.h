#pragma once
#include <raylib.h>

class Player {
    public:
        float player_width{};
        float player_height{};
        Color player_color{};
        Vector2 position{};
        Vector2 previous_position{};        // For frame interpolation
        int movement_speed{};
        Vector2 velocity{};

        Player(float width, float height, Color color, int movement_speed);
        Player() = default;
        void update_physics(float dt);
        void draw(float alpha);

};