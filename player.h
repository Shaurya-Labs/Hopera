#pragma once
#include <raylib.h>

class Player {
    public:
        float width{};
        float height{};
        Color color{};
        Vector2 position{};
        Vector2 previous_position{};        // For frame interpolation
        int movement_speed{};
        Vector2 velocity{};
        bool is_touching_ground{};      // To prevent the physics from updaing and clipping out the player from the ground

        Player(float width, float height, Color color, int movement_speed);
        Player() = default;
        void update_physics(float dt);
        void draw(float alpha);

};