#pragma once
#include <raylib.h>

inline bool debug_mode { false };
inline bool is_left_button_pressed { false };
inline bool is_player_trying_to_run{ false };       // For determining the player's animation states

inline constexpr float ACCELERATION_DUE_TO_GRAVITY { 500.0f };

struct Wall {
    int start_pos_x{};
    int start_pos_y{};
    int end_pos_x{};
    int end_pos_y{};
    
    Color color{};        // For debugging only
};

enum class GameState {
    START_SCREEN,
    PLAYING,
    GAME_OVER
};

inline GameState game_state { GameState::START_SCREEN };