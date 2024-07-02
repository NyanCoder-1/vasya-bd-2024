#pragma once
#include "emsdl.hpp"

class UI
{
public:
    UI();
    ~UI();

    void render();
    void update(const float dtime);

private:
    SDL_Point healthBarPosition;

    // Health Bar
    SDL_Texture *hbTexture = nullptr;
    SDL_Rect hbBarRect, hbBarBackgroundRect, hbHealthBarRect;
    SDL_Point hbBarOffset;

    // Banner
    SDL_Texture *bannerTexture = nullptr;
    float bannerAnimationTime = 0.f;
    static constexpr float bannerAnimationDuration = 5.f;

    // Letter
    SDL_Texture *letterTexture = nullptr;
    SDL_Texture *textTexture = nullptr;
    SDL_Point textSize;
};
