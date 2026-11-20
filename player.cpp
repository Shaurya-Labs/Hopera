#include "player.h"
#include <raylib.h>
#include "raymath.h"
#include "common.h"

Player::Player(float width, float height, Color color, int movement_speed) {
    player_width = width;
    player_height = height;
    player_color = color;
    this->movement_speed = movement_speed;
}

void Player::draw(float alpha) {
    Vector2 render_position = Vector2Lerp(previous_position, position, alpha);
    DrawRectangleV(render_position, Vector2{player_width, player_height}, player_color);
}

void Player::update_physics(float dt) {
    previous_position = position;

    // Update position of the player according to it's velocity
    position.x += velocity.x * movement_speed * dt;

    // Implementing gravity
    velocity.y += ACCELERATION_DUE_TO_GRAVITY * dt;
    position.y += velocity.y * dt;
}