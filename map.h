#pragma once
#include <raylib.h>

class Map {
    public:
        Texture2D map_texture_tile{};

        Map();
        void unload();      // Unloads all textures and audio
        void draw();

    private:
        Rectangle normal_map_rect{};
        Rectangle mirrored_map_rect{};

};