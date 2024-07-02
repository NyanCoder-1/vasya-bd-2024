#pragma once

// types
#include "emsdl.hpp"
#include <memory>
#include "worlds/world.hpp"
#include "entities/character.hpp"
#include "ui.hpp"

enum class WorldId
{
    StartLocation,
};

void GlobalsInit();

// Player
int GetPlayerHealth();
void SetPlayerHealth(int health);
int GetPlayerMaxHealth();
void SetPlayerMaxHealth(int maxHealth);
float GetPlayerHealthRatio();
std::shared_ptr<Entities::Character> GetPlayerCharacter();

// Game
std::shared_ptr<Worlds::World> GetWorld();
void SetWorld(WorldId theWorld);
std::shared_ptr<UI> GetUI();
void SetMouse(int mouseX, int mouseY);
float GetFocusX();
float GetFocusY();

// System
void SetRenderer(SDL_Renderer *theRenderer);
SDL_Renderer *GetRenderer();
void SetScreenSize(int w, int h);
int GetScreenWidth();
int GetScreenHeight();
int GetSceneWidth();
void SetRerender();
void UnsetRerender();
bool IsRerender();
void SetFont(TTF_Font *theFont);
TTF_Font *GetFont();
