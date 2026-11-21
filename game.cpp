#include "game.h"
#include <raylib.h>
#include "player.h"
#include "common.h"
#include <raymath.h>
#include "map.h"

Game::Game(bool debug) {
    debug_mode = debug;
}

void Game::init_player(float width, float height, Color color, int movement_speed) {
    player = Player(width, height, color, movement_speed);
}

void Game::init_camera(Vector2 offset, Vector2 player_initial_position, float rotation, float zoom) {
    camera = {
        .offset = offset,
        .target = player_initial_position,
        .rotation = rotation,
        .zoom = zoom
    };
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

    if (IsKeyPressed(KEY_SPACE) && player.is_touching_ground) {
        player.jump(350.0f);
        player.is_touching_ground = false;
    }
}

void Game::check_and_resolve_collisions() {

    // Player's collision with the walls of the map
    player.is_touching_ground = false;          // Calculate this flag every single game tick

    for (const Wall& wall : walls) {
        bool is_horizontal = (wall.start_pos_y == wall.end_pos_y);
        bool is_vertical = (wall.start_pos_x == wall.end_pos_x);

        if (is_horizontal) {
            // If the player is falling downwards, that means that the player colides with the wall below it
            if ((player.velocity.y>=0) && (player.position.y+player.hitbox_height>=wall.start_pos_y) &&      // Checks if the player is collided with the y pos of the wall while falling down
                (player.position.x+player.hitbox_width>=wall.start_pos_x) && (player.position.x<=wall.end_pos_x) &&        // Checks if the player is within the x-axis range of the horizontal wall(means the player can fall off if it moves off the edge of the horizontal wall)
                (player.previous_position.y+player.hitbox_height<=wall.start_pos_y)) {     // Checks if the player is falling from above the horizontal wall and is not below the wall falling down already
                
                player.position.y = wall.start_pos_y-player.hitbox_height;
                player.velocity.y = 0;
                player.is_touching_ground = true;
            }

        }

        if (is_vertical) {
            // If the player collides with a wall on it's left
            if ((player.velocity.x<0) && (player.position.x<=wall.start_pos_x) && (player.position.x+player.hitbox_width>=wall.start_pos_x)) {
                player.position.x = wall.start_pos_x;
            }

            // IF the player collides with a wall on it's right
            if ((player.velocity.x>0) && (player.position.x+player.hitbox_width>=wall.start_pos_x) && (player.position.x<=wall.start_pos_x)) {
                player.position.x = wall.start_pos_x-player.hitbox_width;
            }

        }

    }

}

void Game::update(float dt) {
    player.update_physics(dt);
    check_and_resolve_collisions();
}

void Game::draw(float alpha) {
    Vector2 render_pos = Vector2Lerp(player.previous_position, player.position, alpha);     // Calculating interpolated frame here for the player to update the camera's target to this frame, so that the motion is smooth
    
    // The whole in game coordinate grid would always be 720px in height, the camera would scale according to it if the screen window
    // even after a window resize, the coordinate grid would be 720px only, the camera would just zoom in/out
    // all the walls and player location would be fixed according to that 720px height grid
    camera.zoom = static_cast<float>(GetScreenHeight()) / 720.0f;
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };      // Calculating camera offset every frame so that it syncs with resizing of the window
    
    // Calculating the total world width of the 2 stitched tiles (mirrored + normal)
    float map_scale = 800.0f / static_cast<float>(map.map_texture_tile.height);
    float total_map_width = (static_cast<float>(map.map_texture_tile.width) * map_scale) * 4.0f;

    // Calculating half of the screen's width in world units
    float half_view_width = (GetScreenWidth() / 2.0f) / camera.zoom;

    // Following the player, then clamp within (half_view_width, total_map_width - half_view_width)
    float target_x = render_pos.x + player.hitbox_width / 2.0f;
    if (total_map_width > half_view_width * 2.0f) {
        target_x = Clamp(target_x, half_view_width, total_map_width - half_view_width);
    } else {
        target_x = total_map_width / 2.0f; // Centers the map if the window is wider than the whole map
    }

    camera.target = {
        target_x,
        720/2
    };

    BeginMode2D(camera);
    map.draw();
    player.draw(alpha);

    // Only for debug mode
    if (debug_mode) {
        for (const Wall& wall : walls) {
            DrawLine(wall.start_pos_x, wall.start_pos_y, wall.end_pos_x, wall.end_pos_y, wall.color);
        }
    }

    EndMode2D();
}

void Game::unload() {
    player.unload();
    map.unload();
}