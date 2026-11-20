#include <raylib.h>
#include <iostream>
#include <chrono>
#include "game.h"

using namespace std::chrono_literals;

int main() {
    std::cout << "Starting the game" << std::endl;

    int WINDOW_WIDTH { 1280 };
    int WINDOW_HEIGHT { 720 };

    std::cout << "Setting the config flags for raylib" << std::endl;
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    std::cout << "Config flags for raylib successfully initialized!" << std::endl;

    std::cout << "Initializing the game window" << std::endl;
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
    std::cout << "Game window initialized successfully!" << std::endl;

    Game game{};
    game.init_player(50, 100, BLUE, 100);

    // Main game loop

    std::chrono::nanoseconds accumulator{};     // Keeps track of how much dt time passed, so that we can compare it to tick_duration
    std::chrono::nanoseconds tick_duration{ 15625000ns };     // Time duration after which one tick should happen, it's 1/64 for our game, 1second/64ticks
    std::chrono::nanoseconds dt{};      // Time duration keeping track of how much time it took to iterate the last outer while loop iteration

    std::chrono::steady_clock::time_point last_update { std::chrono::steady_clock::now() };     // Timestamp which helps to keep track of last loop iteration(with the help of "start" variable) + time taken to loop again to while loop's check

    // This is the outer loop which calculates when the game should tick it's updates and it's iteration frequency = framerate, since this loop draws the frames also
    while (!WindowShouldClose()) {
        
        std::chrono::time_point start { std::chrono::steady_clock::now() };
        dt = start-last_update;
        last_update += dt;
        accumulator += dt;

        // Checking for input once per frame, not once per tick, since that can cause some issues
        game.get_input();

        // This is the while loop which ticks the game, one iteration of this loop = one tick of the game
        while (accumulator >= tick_duration) {
            game.update(std::chrono::duration<float>(tick_duration).count());
            
            accumulator -= tick_duration;
        }

        // This is for frame interpolation, for smooth motion on the screen
        float alpha { static_cast<float>(accumulator.count()) / static_cast<float>(tick_duration.count()) };

        // Drawing the frames
        BeginDrawing();
        ClearBackground(BLACK);

        game.draw(alpha);

        DrawFPS(0, 0);
        EndDrawing();

    }

    CloseWindow();
    return 0;
}