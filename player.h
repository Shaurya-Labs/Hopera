#pragma once
#include <raylib.h>

class Player {
    public:
        float hitbox_width{};
        float hitbox_height{};
        Color color{};
        Vector2 position{};
        Vector2 previous_position{};        // For frame interpolation
        int movement_speed{};
        Vector2 velocity{};
        bool is_touching_ground{};      // To prevent the physics from updaing and clipping out the player from the ground
        Texture2D player_spritesheet_run{};
        Texture2D player_spritesheet_jump{};
        Texture2D player_spritesheet_idle{};
        int facing_direction{ 1 };     // 1 is right, -1 is left
        Music footsteps{};

        Player(float hitbox_width, float hitbox_height, Color color, int movement_speed);
        Player() = default;
        void unload();
        void jump(float intensity);
        void update_physics(float dt);
        void update_music_stream();
        void draw(float alpha);

    private:
        float cell_width{};
        float cell_height{};
        int current_animation_frame{};
        float animation_timer{};

};