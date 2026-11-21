#include "player.h"
#include <raylib.h>
#include "raymath.h"
#include "common.h"

Player::Player(float hitbox_width, float hitbox_height, Color color, int movement_speed) {
    this->hitbox_width = hitbox_width;
    this->hitbox_height = hitbox_height;
    this->color = color;
    this->movement_speed = movement_speed;

    player_spritesheet_run = LoadTexture("assets/graphics/player/Run.png");
    player_spritesheet_jump = LoadTexture("assets/graphics/player/Jump.png");

    cell_width = static_cast<float>(player_spritesheet_run.width) / 6.0f;
    cell_height = static_cast<float>(player_spritesheet_run.height);
}

void Player::unload() {
    // Unload all player textures
    UnloadTexture(player_spritesheet_run);
    UnloadTexture(player_spritesheet_jump);
}

void Player::jump(float intensity) {
    velocity.y -= intensity;
}

void Player::draw(float alpha) {
    Vector2 render_position = Vector2Lerp(previous_position, position, alpha);
    Texture2D& current_spritesheet = is_touching_ground ? player_spritesheet_run : player_spritesheet_jump;

    // The source rectangle window to slice up sprites from the sprite sheet
    Rectangle source {
        current_animation_frame * cell_width,
        0.0f,
        cell_width * facing_direction,
        cell_height
    };

    Rectangle dest {
        render_position.x + (hitbox_width / 2.0f) - (cell_width / 2.0f),
        render_position.y + hitbox_height - cell_height,
        cell_width,
        cell_height
    };

    DrawTexturePro(current_spritesheet, source, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    
    // Only for debug mode
    if (debug_mode) {
        // Drawing the player's hitbox
        DrawRectangleLines(position.x, position.y, hitbox_width, hitbox_height, GREEN);
    }

}

void Player::update_physics(float dt) {
    previous_position = position;

    // Update position of the player according to it's velocity
    if (velocity.x != 0) {
        position.x += velocity.x * movement_speed * dt;
        facing_direction = (velocity.x > 0) ? 1 : -1;
    }

    // Implementing gravity
    if (!is_touching_ground) {
        velocity.y += ACCELERATION_DUE_TO_GRAVITY * dt;
        position.y += velocity.y * dt;
    }

    // For player's sprite animation calculations

    if (!is_touching_ground || (velocity.x!=0)) {
        animation_timer += dt;
        if (animation_timer >= 0.09f) {
            animation_timer -= 0.09f;
            current_animation_frame = (current_animation_frame + 1) % 6;
        }
    }
    
    else {
        current_animation_frame = 0;
        animation_timer = 0.0f;
    }
}