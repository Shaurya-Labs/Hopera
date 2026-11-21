#include "map.h"
#include <raylib.h>
#include "common.h"

Map::Map() {
    // Load all map textures
    map_texture_tile = LoadTexture("assets/graphics/map/map.png");
    npc_sprite_finding = LoadTexture("assets/graphics/npc/npc_finding.png");
    npc_sprite_happy = LoadTexture("assets/graphics/npc/npc_happy.png");
    bike_sprite = LoadTexture("assets/graphics/obstacles/bike.png");
    trash_bin_sprite = LoadTexture("assets/graphics/obstacles/trash_bin.png");
    bush_sprite = LoadTexture("assets/graphics/obstacles/bush.png");
    bag_sprite = LoadTexture("assets/graphics/collectables/bag.png");

    // Load all map music and sounds
    bg_music = LoadMusicStream("assets/audio/bg_music.mp3");
    bg_music.looping = true;
    SetMusicVolume(bg_music, 0.3f);

    item_pickup = LoadSound("assets/audio/item_pick_up.mp3");
    SetSoundVolume(item_pickup, 0.7f);

    task_completed = LoadSound("assets/audio/task_completed.mp3");
    SetSoundVolume(task_completed, 0.7f);

    normal_map_rect = { 0.0f, 0.0f, static_cast<float>(map_texture_tile.width), static_cast<float>(map_texture_tile.height) };
    mirrored_map_rect = { 0.0f, 0.0f, -static_cast<float>(map_texture_tile.width), static_cast<float>(map_texture_tile.height) };

    for (const Rectangle& bike : bikes) {
        int left = static_cast<int>(bike.x);
        int top = static_cast<int>(bike.y);
        int right = static_cast<int>(bike.x + bike.width);
        int bottom = static_cast<int>(bike.y + bike.height);

        // Top surface (horizontal platform to land/stand on)
        walls.push_back(Wall { left, top, right, top, ORANGE });

        // Left side (vertical wall)
        walls.push_back(Wall { left, top, left, bottom, ORANGE });

        // Right side (vertical wall)
        walls.push_back(Wall { right, top, right, bottom, ORANGE });
    }

    for (const Rectangle& trash_bin : trash_bins) {
        int left = static_cast<int>(trash_bin.x);
        int top = static_cast<int>(trash_bin.y);
        int right = static_cast<int>(trash_bin.x + trash_bin.width);
        int bottom = static_cast<int>(trash_bin.y + trash_bin.height);

        // Top surface (horizontal platform to land/stand on)
        walls.push_back(Wall { left, top, right, top, ORANGE });

        // Left side (vertical wall)
        walls.push_back(Wall { left, top, left, bottom, ORANGE });

        // Right side (vertical wall)
        walls.push_back(Wall { right, top, right, bottom, ORANGE });
    }
}

void Map::unload() {
    // Unload all map textures
    UnloadTexture(map_texture_tile);
    UnloadTexture(npc_sprite_finding);
    UnloadTexture(npc_sprite_happy);
    UnloadTexture(bike_sprite);
    UnloadTexture(trash_bin_sprite);
    UnloadTexture(bush_sprite);
    UnloadTexture(bag_sprite);

    // Unload all map music and sounds
    UnloadMusicStream(bg_music);
    UnloadSound(item_pickup);
}

void Map::check_and_resolve_collisions(Camera2D camera) {
    Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), camera);
    
    // Checking for mouse interaction with the NPC
    if (is_left_button_pressed && CheckCollisionPointRec(mouse_world, npc)) {

        if (!bag_captured) {
            show_npc_start_dialogue = true;
        }
        else {
            show_npc_game_won_dialogue = true;
            PlaySound(task_completed);
        }

    }

    // Checking for mouse interaction with the bag
    if (!bag_captured && is_left_button_pressed && CheckCollisionPointRec(mouse_world, reward_bag)) {
        bag_captured = true;
        PlaySound(item_pickup);
    }

}

void Map::update(float dt) {
    if (show_npc_start_dialogue || show_npc_game_won_dialogue) {
        subtitle_timer += dt;
    }

    if (show_npc_start_dialogue) {
        timmer_running = true;
    }

    if (show_npc_game_won_dialogue) {
        timmer_running = false;
    }

    if (timmer_running) {
        timer += dt;
    }
}

void Map::update_music_stream() {
    UpdateMusicStream(bg_music);
}

void Map::draw_static_elements() {
    // Draw the game timer
    int minutes = static_cast<int>(timer) / 60;
    int seconds = static_cast<int>(timer) % 60;

    const char* timer_text = TextFormat("%02d:%02d", minutes, seconds);
    int font_size = 26;
    int text_width = MeasureText(timer_text, font_size);
    int pad_x = 18;
    int pad_y = 8;

    int box_w = text_width + pad_x * 2;
    int box_h = font_size + pad_y * 2;
    int box_x = (GetScreenWidth() - box_w) / 2;
    int box_y = 16;

    DrawRectangle(box_x, box_y, box_w, box_h, Fade(BLACK, 0.75f));
    DrawRectangleLines(box_x, box_y, box_w, box_h, Fade(RAYWHITE, 0.25f));
    DrawText(timer_text, box_x + pad_x, box_y + pad_y, font_size, RAYWHITE);

    // Darw the dialogues
    if (show_npc_start_dialogue) {
        if (subtitle_timer <= 5) {
            draw_dialogues("Elderly Man: I lost my bag down the street, could you help me find it?");
        }
        else {
            subtitle_timer = 0;
            show_npc_start_dialogue = false;
        }
    }
    else if (show_npc_game_won_dialogue) {
        if (subtitle_timer <= 5) {
            draw_dialogues("Elderly Man: Thank you for finding my bag!");
        }
        else {
            subtitle_timer = 0;
            show_npc_game_won_dialogue = false;
            game_state = GameState::GAME_OVER;
            StopMusicStream(bg_music);
        }
    }
}

void Map::draw_dialogues(const char* text, int font_size) {
    int screen_width { GetScreenWidth() };
    int screen_height { GetScreenHeight() };

    int text_width { MeasureText(text, font_size) };
    // Internal text padding for the box
    int padding_x { 24 };
    int padding_y { 12 };

    int box_width { text_width + (padding_x * 2) };
    int box_height { font_size + (padding_y * 2) };
    int box_x { (screen_width - box_width) / 2 };
    int box_y { screen_height - box_height - 48 };      // 48px above the bottom edge of the screen

    DrawRectangle(box_x, box_y, box_width, box_height, Fade(BLACK, 0.78f));
    DrawRectangleLines(box_x, box_y, box_width, box_height, Fade(RAYWHITE, 0.25f));

    int text_x { box_x + padding_x };
    int text_y { box_y + padding_y };

    DrawText(text, text_x, text_y, font_size, RAYWHITE);

}

void Map::draw_sprite(const Texture2D& texture, const Rectangle& hitbox, float target_height, Color tint) {

    float scale = target_height / static_cast<float>(texture.height);
    float draw_width = static_cast<float>(texture.width) * scale;

    Rectangle source {
        0.0f,
        0.0f,
        static_cast<float>(texture.width),
        static_cast<float>(texture.height)
    };

    Rectangle dest {
        hitbox.x + (hitbox.width / 2.0f) - (draw_width / 2.0f),
        hitbox.y + hitbox.height - target_height,
        draw_width,
        target_height
    };

    DrawTexturePro(texture, source, dest, Vector2{0.0f, 0.0f}, 0.0f, tint);
}

void Map::draw() {

    // Draw the map
    float scale = 800.0f / static_cast<float>(map_texture_tile.height);
    float draw_width = static_cast<float>(map_texture_tile.width) * scale;
    float draw_height = 800.0f;

    DrawTexturePro(map_texture_tile, mirrored_map_rect, Rectangle{ 0.0f, 0.0f, draw_width, draw_height }, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(map_texture_tile, normal_map_rect, Rectangle{ draw_width, 0.0f, draw_width, draw_height }, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(map_texture_tile, mirrored_map_rect, Rectangle{ draw_width*2, 0.0f, draw_width, draw_height }, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    DrawTexturePro(map_texture_tile, normal_map_rect, Rectangle{ draw_width*3, 0.0f, draw_width, draw_height }, Vector2{0.0f, 0.0f}, 0.0f, WHITE);

    // Draw the npc
    Texture2D* npc_sprite_to_draw { show_npc_game_won_dialogue ? &npc_sprite_happy : &npc_sprite_finding };
    float npc_draw_height { show_npc_game_won_dialogue ? 68.0f : 55.0f };
    float npc_scale = npc_draw_height / static_cast<float>(npc_sprite_finding.height);
    float npc_draw_width = static_cast<float>(npc_sprite_finding.width) * npc_scale;

    Rectangle source {
        0.0f,
        0.0f,
        static_cast<float>(npc_sprite_to_draw->width),
        static_cast<float>(npc_sprite_to_draw->height)
    };

    Rectangle dest {
        npc.x + (npc.width / 2.0f) - (npc_draw_width / 2.0f),
        npc.y + npc.height - npc_draw_height,
        npc_draw_width,
        npc_draw_height
    };

    DrawTexturePro(*npc_sprite_to_draw, source, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);


    // Draw obstacles
    for (const Rectangle& bike : bikes) {
        draw_sprite(bike_sprite, bike, 52.0f);
    }

    for (const Rectangle& trash_bin : trash_bins) {
        draw_sprite(trash_bin_sprite, trash_bin, 46.0f);
    }

    for (const Rectangle& bush : bushes) {
        draw_sprite(bush_sprite, bush, 56.0f);
    }

    // Draw the reward bag
    if (!bag_captured) {
        draw_sprite(bag_sprite, reward_bag, 40.0f);
    }

}