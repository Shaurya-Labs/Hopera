#include "game.h"
#include <raylib.h>
#include "player.h"
#include "common.h"

void Game::init_player(float width, float height, Color color, int movement_speed) {
    player = Player(width, height, color, movement_speed);
}

void Game::get_input() {
    if (IsKeyDown(KEY_A)) {
        player.velocity.x = -1;
    }

    else if (IsKeyDown(KEY_D)) {
        player.velocity.x = 1;
    }

    else {
        player.velocity.x = 0;
    }
}

void Game::update(float dt) {
    player.update_physics(dt);
}

void Game::draw(float alpha) {
    player.draw(alpha);
}