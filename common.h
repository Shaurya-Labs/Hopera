#pragma once
#include <raylib.h>

inline bool debug_mode { false };
inline bool is_left_button_pressed { false };

inline constexpr float ACCELERATION_DUE_TO_GRAVITY { 500.0f };

struct Wall {
    int start_pos_x{};
    int start_pos_y{};
    int end_pos_x{};
    int end_pos_y{};
    
    Color color{};        // For debugging only
};