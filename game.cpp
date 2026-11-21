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
    is_player_trying_to_run = false;

    // For Keyboard
    if (IsKeyDown(KEY_A)) {
        player.velocity.x = -1;
        is_player_trying_to_run = true;
    }
    else if (IsKeyDown(KEY_D)) {
        player.velocity.x = 1;
        is_player_trying_to_run = true;
    }
    else {
        player.velocity.x = 0;
    }

    if (IsKeyPressed(KEY_SPACE) && player.is_touching_ground) {
        player.jump(250.0f);
        player.is_touching_ground = false;
    }

    // For mouse
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        is_left_button_pressed = true;
    }
}

void Game::check_and_resolve_collisions() {

    // Player's collision with the walls of the map
    player.is_touching_ground = false;          // Calculate this flag every single game tick

    for (const Wall& wall : map.walls) {
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
            if ((player.velocity.x<0) && (player.position.x<=wall.start_pos_x) && (player.position.x+player.hitbox_width>wall.start_pos_x) &&
                (player.position.y+player.hitbox_height>wall.start_pos_y) && (player.position.y<wall.end_pos_y)) {
                player.position.x = wall.start_pos_x;
                player.velocity.x = 0;
            }

            // IF the player collides with a wall on it's right
            if ((player.velocity.x>0) && (player.position.x+player.hitbox_width>=wall.start_pos_x) && (player.position.x<wall.start_pos_x) &&
                (player.position.y+player.hitbox_height>wall.start_pos_y) && (player.position.y<wall.end_pos_y)) {
                player.position.x = wall.start_pos_x-player.hitbox_width;
                player.velocity.x = 0;
            }
        }

    }

}

void Game::update(float dt) {

    // For the initial start screen
    if (game_state == GameState::START_SCREEN) {
        int screen_width = GetScreenWidth();
        int screen_height = GetScreenHeight();

        Rectangle start_btn {
            static_cast<float>(screen_width) / 2.0f - 110.0f,
            static_cast<float>(screen_height) / 2.0f + 50.0f,
            220.0f,
            56.0f
        };

        Vector2 mouse_pos = GetMousePosition();
        if (is_left_button_pressed && CheckCollisionPointRec(mouse_pos, start_btn)) {
            game_state = GameState::PLAYING;
            PlayMusicStream(map.bg_music);
        }

        return;
    }

    // For game over screen
    if (game_state == GameState::GAME_OVER) {
        int screen_width = GetScreenWidth();
        int screen_height = GetScreenHeight();

        Rectangle play_again_btn {
            static_cast<float>(screen_width) / 2.0f - 110.0f,
            static_cast<float>(screen_height) / 2.0f + 70.0f,
            220.0f,
            56.0f
        };

        Vector2 mouse_pos = GetMousePosition();
        if (is_left_button_pressed && CheckCollisionPointRec(mouse_pos, play_again_btn)) {
            map.timer = 0.0f;
            map.bag_captured = false;
            map.timmer_running = false;
            player.position = Vector2{ 80, 500 };
            player.previous_position = player.position;
            player.velocity = { 0.0f, 0.0f };
            player.facing_direction = 1;
            game_state = GameState::PLAYING;
            PlayMusicStream(map.bg_music);
        }

        is_left_button_pressed = false;
        return;
    }

    player.update_physics(dt);
    check_and_resolve_collisions();

    map.update(dt);
    map.check_and_resolve_collisions(camera);

    // Resetting the flags after handling them
    is_left_button_pressed = false;     // Not inside get_input() function, since that function is polled once per frame, not once per tick, so the flag can be left unhandled
}

void Game::update_music_stream() {
    player.update_music_stream();
    map.update_music_stream();
}

void Game::draw(float alpha) {

    // For initial start screen
    if (game_state == GameState::START_SCREEN) {
        int screen_width = GetScreenWidth();
        int screen_height = GetScreenHeight();

        const char* title = "HOPERA";
        const char* prompt = "Help the old man retrieve his bag from the end of the map";
        const char* btn_label = "START";

        int title_size = 48;
        int prompt_size = 24;
        int btn_text_size = 26;

        int title_width = MeasureText(title, title_size);
        int prompt_width = MeasureText(prompt, prompt_size);
        int btn_text_width = MeasureText(btn_label, btn_text_size);

        Rectangle start_btn {
            static_cast<float>(screen_width) / 2.0f - 110.0f,
            static_cast<float>(screen_height) / 2.0f + 50.0f,
            220.0f,
            56.0f
        };

        bool hovered = CheckCollisionPointRec(GetMousePosition(), start_btn);

        DrawRectangle(0, 0, screen_width, screen_height, Color{ 14, 17, 26, 255 });

        DrawText(title, (screen_width - title_width) / 2, screen_height / 2 - 120, title_size, RAYWHITE);
        DrawText(prompt, (screen_width - prompt_width) / 2, screen_height / 2 - 40, prompt_size, LIGHTGRAY);

        DrawRectangleRec(start_btn, hovered ? SKYBLUE : DARKBLUE);
        DrawRectangleLinesEx(start_btn, 2.0f, RAYWHITE);
        DrawText(
            btn_label,
            static_cast<int>(start_btn.x + (start_btn.width - btn_text_width) / 2.0f),
            static_cast<int>(start_btn.y + (start_btn.height - btn_text_size) / 2.0f),
            btn_text_size,
            RAYWHITE
        );

        return;
    }

    // For game over screen
    if (game_state == GameState::GAME_OVER) {
        int screen_width = GetScreenWidth();
        int screen_height = GetScreenHeight();

        int minutes = static_cast<int>(map.timer) / 60;
        int seconds = static_cast<int>(map.timer) % 60;

        const char* heading = "YOU FOUND THE BAG!";
        const char* score_text = TextFormat("Time Score: %02d:%02d", minutes, seconds);
        const char* btn_label = "PLAY AGAIN";

        int heading_size = 46;
        int score_size = 28;
        int btn_text_size = 24;

        int heading_width = MeasureText(heading, heading_size);
        int score_width = MeasureText(score_text, score_size);
        int btn_text_width = MeasureText(btn_label, btn_text_size);

        Rectangle play_again_btn {
            static_cast<float>(screen_width) / 2.0f - 110.0f,
            static_cast<float>(screen_height) / 2.0f + 70.0f,
            220.0f,
            56.0f
        };

        bool hovered = CheckCollisionPointRec(GetMousePosition(), play_again_btn);

        DrawRectangle(0, 0, screen_width, screen_height, Color{ 14, 17, 26, 255 });
        DrawText(heading, (screen_width - heading_width) / 2, screen_height / 2 - 120, heading_size, RAYWHITE);
        DrawText(score_text, (screen_width - score_width) / 2, screen_height / 2 - 30, score_size, GOLD);

        DrawRectangleRec(play_again_btn, hovered ? SKYBLUE : DARKBLUE);
        DrawRectangleLinesEx(play_again_btn, 2.0f, RAYWHITE);
        DrawText(
            btn_label,
            static_cast<int>(play_again_btn.x + (play_again_btn.width - btn_text_width) / 2.0f),
            static_cast<int>(play_again_btn.y + (play_again_btn.height - btn_text_size) / 2.0f),
            btn_text_size,
            RAYWHITE
        );

        return;
    }

    // Gameplay
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

    // The coordinates which the camera would be following
    camera.target = {
        target_x,
        720/2
    };

    BeginMode2D(camera);
    map.draw();
    player.draw(alpha);

    // Only for debug mode
    if (debug_mode) {
        for (const Wall& wall : map.walls) {
            DrawLine(wall.start_pos_x, wall.start_pos_y, wall.end_pos_x, wall.end_pos_y, wall.color);
        }

        Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), camera);
        int mouse_x = static_cast<int>(mouse_world.x);
        int mouse_y = static_cast<int>(mouse_world.y);

        // Small circle marker at the exact world point at the cursor
        DrawCircleLines(mouse_x, mouse_y, 4.0f, RED);

        // Text offset slightly so it doesn't sit right under the pointer
        DrawText(TextFormat("X: %d, Y: %d", mouse_x, mouse_y), mouse_x + 12, mouse_y - 24, 20, BLACK);
    }

    EndMode2D();

    map.draw_static_elements();
}

void Game::unload() {
    player.unload();
    map.unload();
}