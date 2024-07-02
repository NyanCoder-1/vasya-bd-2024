#include "ui.hpp"
#include "globals.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

UI::UI()
{
    auto atlasUiPath = std::filesystem::current_path()/"assets"/"atlas_health_bar.png";
    hbTexture = IMG_LoadTexture(GetRenderer(), atlasUiPath.c_str());
    if (!hbTexture) {
        std::cerr << "Couldn't open atlas texture file: " << atlasUiPath.c_str() << " with error \"" << TTF_GetError() << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto atlasUiJsonPath = std::filesystem::current_path()/"assets"/"atlas_health_bar.json";
    std::ifstream atlasUiJsonFile(atlasUiJsonPath);
    nlohmann::json atlasUiJson = nlohmann::json::parse(atlasUiJsonFile);
    auto atlasUiBarRect = atlasUiJson["bar"];
    auto atlasUiBarBackgroundRect = atlasUiJson["bar_background"];
    auto atlasUiHealthBarRect = atlasUiJson["health_bar"];
    hbBarRect = {
        .x = static_cast<int>(atlasUiBarRect["x"]),
        .y = static_cast<int>(atlasUiBarRect["y"]),
        .w = static_cast<int>(atlasUiBarRect["w"]),
        .h = static_cast<int>(atlasUiBarRect["h"])
    };
    hbBarBackgroundRect = {
        .x = static_cast<int>(atlasUiBarBackgroundRect["x"]),
        .y = static_cast<int>(atlasUiBarBackgroundRect["y"]),
        .w = static_cast<int>(atlasUiBarBackgroundRect["w"]),
        .h = static_cast<int>(atlasUiBarBackgroundRect["h"])
    };
    hbHealthBarRect = {
        .x = static_cast<int>(atlasUiHealthBarRect["x"]),
        .y = static_cast<int>(atlasUiHealthBarRect["y"]),
        .w = static_cast<int>(atlasUiHealthBarRect["w"]),
        .h = static_cast<int>(atlasUiHealthBarRect["h"])
    };

    bannerTexture = IMG_LoadTexture(GetRenderer(), (std::filesystem::current_path()/"assets"/"vasya-banner.png").c_str());
    letterTexture = IMG_LoadTexture(GetRenderer(), (std::filesystem::current_path()/"assets"/"scroll.png").c_str());

    static const std::string text = "Йо! Ma best friend Вася!\n\nТөрөөбүт күҥҥүнэн итиитик истинник эҕэрдэлиибин!\n\nЩастья-здаровья, чтоб сил на все дела хватило, чтоб никогда сам не выгорал и чтоб выгорали все дела. Кста, ещё хорошо высыпайся\n\nНе скучай\n\nСНП, Витя.";

    SDL_Surface *textSurface = TTF_RenderUTF8_Blended_Wrapped(GetFont(), text.c_str(), {0, 0, 0, 255}, 768);
    textTexture = SDL_CreateTextureFromSurface(GetRenderer(), textSurface);
    textSize = { .x = textSurface->w, .y = textSurface->h };
    SDL_FreeSurface(textSurface);
}
UI::~UI()
{
    if (hbTexture) {
        SDL_DestroyTexture(hbTexture);
        hbTexture = nullptr;
    }
    if (bannerTexture) {
        SDL_DestroyTexture(bannerTexture);
        bannerTexture = nullptr;
    }
    if (letterTexture) {
        SDL_DestroyTexture(letterTexture);
        letterTexture = nullptr;
    }
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
}

void UI::render()
{
    constexpr auto letterWidth = 384;
    constexpr auto letterHeight = 480;
    const auto letterMultiplier = static_cast<float>(GetScreenHeight()) / letterHeight * 0.8f;
    SDL_Rect letterRect = { .x = static_cast<int>(GetScreenWidth() - letterWidth * letterMultiplier) / 2, .y = static_cast<int>(GetScreenHeight() * (0.1f + std::sinf(bannerAnimationTime * 2 * M_PI / bannerAnimationDuration + M_PI*0.1f) * 0.02f)), .w = static_cast<int>(letterWidth * letterMultiplier), .h = static_cast<int>(letterHeight * letterMultiplier) };
    SDL_RenderCopy(GetRenderer(), letterTexture, nullptr, &letterRect);

    constexpr auto textOffsetX = 64/12*28;
    constexpr auto textOffsetY = 120;
    const auto textWidth = static_cast<int>(textSize.x * 320 / 768.f);
    const auto textHeight = static_cast<int>(static_cast<float>(textWidth) / textSize.x * textSize.y);
    constexpr auto textMultiplier = 0.8f;
    SDL_Rect textRect = { .x = static_cast<int>(GetScreenWidth() - (letterWidth - textOffsetX) * letterMultiplier) / 2, .y = static_cast<int>(GetScreenHeight() * (0.1f + std::sinf(bannerAnimationTime * 2 * M_PI / bannerAnimationDuration + M_PI*0.1f) * 0.02f) + textOffsetY * letterMultiplier), .w = static_cast<int>(textWidth * letterMultiplier * textMultiplier), .h = static_cast<int>(textHeight * letterMultiplier * textMultiplier) };
    SDL_RenderCopy(GetRenderer(), textTexture, nullptr, &textRect);

    constexpr auto bannerWidth = 230;
    constexpr auto bannerHeight = 59;
    const auto bannerMultiplier = static_cast<float>(GetScreenHeight()) / bannerHeight / 6;
    SDL_Rect bannerRect = { .x = static_cast<int>(GetScreenWidth() - bannerWidth * bannerMultiplier) / 2, .y = static_cast<int>(GetScreenHeight() * (0.05f + std::sinf(bannerAnimationTime * 2 * M_PI / bannerAnimationDuration) * 0.02f)), .w = static_cast<int>(bannerWidth * bannerMultiplier), .h = static_cast<int>(bannerHeight * bannerMultiplier) };
    SDL_RenderCopy(GetRenderer(), bannerTexture, nullptr, &bannerRect);
}

void UI::update(const float dtime)
{
    bannerAnimationTime += dtime;
    bannerAnimationTime = std::fmod(bannerAnimationTime, bannerAnimationDuration);
}
