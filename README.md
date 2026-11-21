# Hopera

A 2D street parkour game built from scratch in C++ and Raylib for **Lean In Hacks 8.0** (Team **Creative Hackers**, Beginner's Track). 

An elderly man loses his bag down a busy street, and you have to hop across parked bikes and trash bins to reach the end of the road, grab the bag, and bring it back as fast as possible.

> **Demo Video:** [Watch Gameplay on YouTube](https://youtu.be/h8KY7LKlBO8)

## Playing the Game

1. Download the latest `.zip` from the [Releases](../../releases) page.
2. Extract all files (keep `hopera.exe` in the same folder as `assets/`).
3. Run `hopera.exe`.

## Controls

- **`A` / `D`** — Move left / right
- **`Space`** — Jump
- **`Left Click`** — Talk to the elderly man / pick up the bag / interact with menu buttons

## How It's Built

Instead of using an existing game engine like Unity or Godot, the game runs on a custom 2D game engine written in C++ using Raylib for windowing, textures, and audio:

- **Fixed-Timestep Game Loop:** Physics and game logic run at a fixed **64 Hz** tickrate using `std::chrono`, decoupled from rendering frame rate with **frame interpolation** (`Vector2Lerp`) so movement stays smooth across monitors.
- **Custom Collision Detection:** Custom logic for collision handling, separate horizontal top surfaces (for landing on obstacles) and vertical side walls.
- **Camera & Scaling:** 2D camera tracking clamped to the map boundaries with automatic scaling to a 720p virtual vertical grid on window resize.
- **Audio & Animation:** 6-frame sprite sheet animations (`Idle`, `Run`, `Jump`), looping background music, footsteps, and pickup sound effects.
- **Art:** Sprites and map assets edited and drawn in Krita.
