#pragma once

#include "world.hpp"
#include "emsdl.hpp"
#include <unordered_map>
#include <vector>

namespace Worlds {
    class StartLocation : public World {
    public:
        enum class TileId {
            nothing, // air

            id_begin,

            grass_single = id_begin,
            grass,
            grass_bottomless,
            grass_left_bottomless,
            grass_right_bottomless,
            grass_pillar_top,
            grass_left,
            grass_right,
            pillar,
            pillar_bottom,
            ground_left,
            ground_right,
            ground,
            ground_bottom,
            ground_bottom_left,
            ground_bottom_right,
            grass_slope_0,
            grass_slope_1,
            grass_slope_2,
            grass_slope_down_0,
            grass_slope_down_1,
            grass_slope_down_2,
            stone_single,
            stone,
            stone_bottomless,
            stone_left_bottomless,
            stone_right_bottomless,
            stone_pillar_top,
            stone_left,
            stone_right,
            stone_to_grass,
            grass_to_stone,
            grass_ends_to_ground,
            grass_starts_from_ground,

            id_end
        };
    public:
        StartLocation();
        ~StartLocation() override;
        void render() override;
        void update(const float dtime) override;

    private:
        static constexpr auto bgLayers = 3;
        SDL_Texture *bg[bgLayers] = {nullptr};
        SDL_Texture *tileset = nullptr;
        int tileWidth = 16;
        int tileHeight = 16;
        std::unordered_map<TileId, std::vector<SDL_Rect>> tileMap;

        static constexpr auto shopAnimationDuration = 0.5f;
        float shopAnimationTime = 0;

        SDL_Surface *fgSurface = nullptr;
        SDL_Texture *fgTexture = nullptr;

        // Decorations;
        static constexpr auto shopWidth = 118;
        static constexpr auto shopFrameCount = 6;
        SDL_Texture *shop = nullptr;
        static constexpr auto grassCount = 3;
        SDL_Texture *grass[grassCount] = {nullptr};
        static constexpr auto fenceCount = 2;
        SDL_Texture *fence[fenceCount] = {nullptr};
        static constexpr auto rockCount = 3;
        SDL_Texture *rock[rockCount] = {nullptr};
        SDL_Texture *sign = nullptr;
        SDL_Texture *lamp = nullptr;
    };
}