#include "startLocation.hpp"
#include "globals.hpp"
#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <nlohmann/json.hpp>

namespace {
    std::string getTileName(Worlds::StartLocation::TileId id){
        switch (id) {
            case Worlds::StartLocation::TileId::grass_single:             return "grass_single";
            case Worlds::StartLocation::TileId::grass:                    return "grass";
            case Worlds::StartLocation::TileId::grass_bottomless:         return "grass_bottomless";
            case Worlds::StartLocation::TileId::grass_left_bottomless:    return "grass_left_bottomless";
            case Worlds::StartLocation::TileId::grass_right_bottomless:   return "grass_right_bottomless";
            case Worlds::StartLocation::TileId::grass_pillar_top:         return "grass_pillar_top";
            case Worlds::StartLocation::TileId::grass_left:               return "grass_left";
            case Worlds::StartLocation::TileId::grass_right:              return "grass_right";
            case Worlds::StartLocation::TileId::pillar:                   return "pillar";
            case Worlds::StartLocation::TileId::pillar_bottom:            return "pillar_bottom";
            case Worlds::StartLocation::TileId::ground_left:              return "ground_left";
            case Worlds::StartLocation::TileId::ground_right:             return "ground_right";
            case Worlds::StartLocation::TileId::ground:                   return "ground";
            case Worlds::StartLocation::TileId::ground_bottom:            return "ground_bottom";
            case Worlds::StartLocation::TileId::ground_bottom_left:       return "ground_bottom_left";
            case Worlds::StartLocation::TileId::ground_bottom_right:      return "ground_bottom_right";
            case Worlds::StartLocation::TileId::grass_slope_0:            return "grass_slope_0";
            case Worlds::StartLocation::TileId::grass_slope_1:            return "grass_slope_1";
            case Worlds::StartLocation::TileId::grass_slope_2:            return "grass_slope_2";
            case Worlds::StartLocation::TileId::grass_slope_down_0:       return "grass_slope_down_0";
            case Worlds::StartLocation::TileId::grass_slope_down_1:       return "grass_slope_down_1";
            case Worlds::StartLocation::TileId::grass_slope_down_2:       return "grass_slope_down_2";
            case Worlds::StartLocation::TileId::stone_single:             return "stone_single";
            case Worlds::StartLocation::TileId::stone:                    return "stone";
            case Worlds::StartLocation::TileId::stone_bottomless:         return "stone_bottomless";
            case Worlds::StartLocation::TileId::stone_left_bottomless:    return "stone_left_bottomless";
            case Worlds::StartLocation::TileId::stone_right_bottomless:   return "stone_right_bottomless";
            case Worlds::StartLocation::TileId::stone_pillar_top:         return "stone_pillar_top";
            case Worlds::StartLocation::TileId::stone_left:               return "stone_left";
            case Worlds::StartLocation::TileId::stone_right:              return "stone_right";
            case Worlds::StartLocation::TileId::stone_to_grass:           return "stone_to_grass";
            case Worlds::StartLocation::TileId::grass_to_stone:           return "grass_to_stone";
            case Worlds::StartLocation::TileId::grass_ends_to_ground:     return "grass_ends_to_ground";
            case Worlds::StartLocation::TileId::grass_starts_from_ground: return "grass_starts_from_ground";
        }
        return "";
    }
    using TileId = Worlds::StartLocation::TileId;
    const std::vector<std::array<std::tuple<Worlds::StartLocation::TileId, size_t>, 8>> mapTiles = {
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {TileId::grass_bottomless, 1}, {TileId::grass_starts_from_ground, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {TileId::grass_bottomless, 0}, {TileId::grass_bottomless, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {TileId::grass_right_bottomless, 0}, {TileId::grass_to_stone, 1}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::stone_bottomless, 1}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::stone_bottomless, 1}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::stone_bottomless, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::stone_to_grass, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::grass_to_stone, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::stone_bottomless, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::stone_to_grass, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::grass_bottomless, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::grass_bottomless, 1}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::grass_bottomless, 1}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::grass_bottomless, 0}}},
        std::array<std::tuple<TileId, size_t>, 8>{{{}, {}, {}, {}, {}, {}, {}, {TileId::grass_bottomless, 1}}},
    };
}

Worlds::StartLocation::StartLocation()
{
    auto pathTilesetJson = std::filesystem::current_path() / "assets" / "oak_woods_tileset.json";
    std::ifstream file(pathTilesetJson.c_str());
    if (!file) {
        std::cerr << "Couldn't open " << pathTilesetJson.c_str() << std::endl;
        exit(EXIT_FAILURE);
    }
    auto json = nlohmann::json::parse(file);
    tileWidth = static_cast<int>(json["tile_size"]["w"]);
    tileHeight = static_cast<int>(json["tile_size"]["h"]);
    for (auto id = Worlds::StartLocation::TileId::id_begin; id < Worlds::StartLocation::TileId::id_end; (*reinterpret_cast<size_t*>(&id))++) {
        for (auto tile : json[getTileName(id)].items()) {
            auto x = static_cast<int>(tile.value()[0]);
            auto y = static_cast<int>(tile.value()[1]);
            tileMap[id].emplace_back(SDL_Rect{ .x = x * tileWidth, .y = y * tileHeight, .w = tileWidth, .h = tileHeight});
        }
    }

    fgSurface = SDL_CreateRGBSurface(0, tileWidth * mapTiles.size(), tileHeight * mapTiles[0].size(), 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    SDL_SetSurfaceBlendMode(fgSurface, SDL_BLENDMODE_BLEND);
    SDL_Renderer *fgRenderer = SDL_CreateSoftwareRenderer(fgSurface);
    SDL_BlendMode(SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(fgRenderer, 0, 0, 0, 0);
    SDL_RenderClear(fgRenderer);

    // Load textures for fg
    auto pathTilesetPng = std::filesystem::current_path() / "assets" / "oak_woods_tileset.png";
    tileset = IMG_LoadTexture(fgRenderer, pathTilesetPng.c_str());
    fence[0] = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "fence_1.png").c_str());
    fence[1] = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "fence_2.png").c_str());
    grass[0] = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "grass_1.png").c_str());
    grass[1] = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "grass_2.png").c_str());
    grass[2] = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "grass_3.png").c_str());
    lamp = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "lamp.png").c_str());
    rock[0] = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "rock_1.png").c_str());
    rock[1] = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "rock_2.png").c_str());
    rock[2] = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "rock_3.png").c_str());
    sign = IMG_LoadTexture(fgRenderer, (std::filesystem::current_path() / "assets" / "sign.png").c_str());

    // Draw decorations
    SDL_Rect decorationRect = { .x = 163, .y = 163, .w = 10, .h = 5 };
    SDL_RenderCopy(fgRenderer, grass[1], nullptr, &decorationRect);
    decorationRect = { .x = 14, .y = 132, .w = 27, .h = 12 };
    SDL_RenderCopy(fgRenderer, rock[1], nullptr, &decorationRect);

    // Prepare tiles
    constexpr auto offsetX = -6;
    constexpr auto offsetY = -5;
    for (auto columnNum = 0; columnNum < mapTiles.size(); columnNum++) {
        const auto &column = mapTiles[columnNum];
        for (auto rowNum = 0; rowNum < column.size(); rowNum++) {
            const auto &[tileId, variant] = column[rowNum];
            if (tileId == TileId::nothing) {
                continue;
            }
            SDL_Rect tileRect = { .x = columnNum * tileWidth, .y = rowNum * tileHeight, .w = tileWidth, .h = tileHeight };
            SDL_RenderCopy(fgRenderer, tileset, &tileMap[tileId][variant % tileMap[tileId].size()], &tileRect);
        }
    }

    SDL_DestroyTexture(tileset);
    tileset = nullptr;
    SDL_DestroyRenderer(fgRenderer);
    fgTexture = SDL_CreateTextureFromSurface(GetRenderer(), fgSurface);

    bg[0] = IMG_LoadTexture(GetRenderer(), (std::filesystem::current_path() / "assets" / "background_layer_1.png").c_str());
    bg[1] = IMG_LoadTexture(GetRenderer(), (std::filesystem::current_path() / "assets" / "background_layer_2.png").c_str());
    bg[2] = IMG_LoadTexture(GetRenderer(), (std::filesystem::current_path() / "assets" / "background_layer_3.png").c_str());

    shop = IMG_LoadTexture(GetRenderer(), (std::filesystem::current_path() / "assets" / "shop_anim.png").c_str());
}

Worlds::StartLocation::~StartLocation()
{
    if (fgTexture) {
        SDL_DestroyTexture(fgTexture);
        fgTexture = nullptr;
    }
    if (fgSurface) {
        SDL_FreeSurface(fgSurface);
        fgSurface = nullptr;
    }
    for (int i = 0; i < bgLayers; i++) {
        if (bg[i]) {
            SDL_DestroyTexture(bg[i]);
            bg[i] = nullptr;
        }
    }
    if (shop) {
        SDL_DestroyTexture(shop);
        shop = nullptr;
    }
    for (int i = 0; i < grassCount; i++) {
        if (grass[i]) {
            SDL_DestroyTexture(grass[i]);
            grass[i] = nullptr;
        }
    }
    for (int i = 0; i < fenceCount; i++) {
        if (fence[i]) {
            SDL_DestroyTexture(fence[i]);
            fence[i] = nullptr;
        }
    }
    for (int i = 0; i < rockCount; i++) {
        if (rock[i]) {
            SDL_DestroyTexture(rock[i]);
            rock[i] = nullptr;
        }
    }
    if (sign) {
        SDL_DestroyTexture(sign);
        sign = nullptr;
    }
}

void Worlds::StartLocation::render()
{
    // const auto multiplier = GetSceneWidth() / (12.f * tileWidth);
    const auto multiplier = GetScreenHeight() / (8.f * tileHeight);
    const auto fgTextureWidth = fgSurface->w;
    const auto fgTextureHeight = fgSurface->h;
    constexpr auto bgTextureWidth = 320;
    constexpr auto bgTextureHeight = 180;
    const auto targetScreenOffset = (GetScreenWidth() - GetSceneWidth()) / 2;
    const auto targetTextureOffset = (GetScreenWidth() - static_cast<float>(fgTextureWidth) / fgTextureHeight * GetScreenHeight()) / 2;
    const int offsetX =  targetScreenOffset - (static_cast<float>(fgTextureWidth) / fgTextureHeight * GetScreenHeight() - GetSceneWidth()) * GetFocusX();
    constexpr auto offsetY = 0;

    SDL_Rect dstRect;

    const auto bgTextureOnScreenWidth = static_cast<int>(static_cast<float>(bgTextureWidth) / bgTextureHeight * GetScreenHeight());
    const auto bgTextureOnScreenHeight = GetScreenHeight();
    const auto bg1MaxOffsetX = static_cast<int>(5 * GetScreenHeight() / bgTextureHeight);
    dstRect = { .x = (GetScreenWidth() - bgTextureOnScreenWidth) / 2 - static_cast<int>(bg1MaxOffsetX * GetFocusX()), .y = 0, .w = bgTextureOnScreenWidth, .h = bgTextureOnScreenHeight };
    SDL_RenderCopy(GetRenderer(), bg[0], nullptr, &dstRect);
    dstRect.x += bgTextureOnScreenWidth;
    SDL_RenderCopy(GetRenderer(), bg[0], nullptr, &dstRect);
    const auto bg2MaxOffsetX = static_cast<int>(8 * GetScreenHeight() / bgTextureHeight);
    dstRect = { .x = (GetScreenWidth() - bgTextureOnScreenWidth) / 2 - static_cast<int>(bg2MaxOffsetX * GetFocusX()), .y = 0, .w = bgTextureOnScreenWidth, .h = bgTextureOnScreenHeight };
    SDL_RenderCopy(GetRenderer(), bg[1], nullptr, &dstRect);
    dstRect.x += bgTextureOnScreenWidth;
    SDL_RenderCopy(GetRenderer(), bg[1], nullptr, &dstRect);
    const auto bg3MaxOffsetX = static_cast<int>(10 * GetScreenHeight() / bgTextureHeight);
    dstRect = { .x = (GetScreenWidth() - bgTextureOnScreenWidth) / 2 - static_cast<int>(bg3MaxOffsetX * GetFocusX()), .y = 0, .w = bgTextureOnScreenWidth, .h = bgTextureOnScreenHeight };
    SDL_RenderCopy(GetRenderer(), bg[2], nullptr, &dstRect);
    dstRect.x += bgTextureOnScreenWidth;
    SDL_RenderCopy(GetRenderer(), bg[2], nullptr, &dstRect);

    constexpr auto shopPositionX = 234;
    constexpr auto shopPositionY = 40;
    const auto shopMultiplier = static_cast<float>(GetScreenHeight()) / fgTextureHeight;
    dstRect = { .x = static_cast<int>(shopPositionX * shopMultiplier) + offsetX, .y = static_cast<int>(shopPositionY * shopMultiplier) + offsetY, .w = static_cast<int>(118 * shopMultiplier), .h = static_cast<int>(128 * shopMultiplier) };
    SDL_Rect srcRect = { .x = static_cast<int>(std::clamp(shopAnimationTime / shopAnimationDuration * 6, 0.f, 5.f)) * 118, .y = 0, .w = 118, .h = 128 };
    //SDL_RenderCopy(GetRenderer(), shop, &srcRect, nullptr);
    SDL_RenderCopy(GetRenderer(), shop, &srcRect, &dstRect);
    int w, h;
    SDL_QueryTexture(fgTexture, nullptr, nullptr, &w, &h);
    std::cout << srcRect.x << " " << srcRect.y << " " << srcRect.w << " " << srcRect.h << "    \r";

    dstRect = { .x = offsetX, .y = offsetY, .w = static_cast<int>(static_cast<float>(fgTextureWidth) / fgTextureHeight * GetScreenHeight()), .h = GetScreenHeight() };
    SDL_RenderCopy(GetRenderer(), fgTexture, nullptr, &dstRect);
}

void Worlds::StartLocation::update(const float dtime)
{
    shopAnimationTime += dtime;
    shopAnimationTime = std::fmod(shopAnimationTime, shopAnimationDuration);
}