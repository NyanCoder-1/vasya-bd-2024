#include "globals.hpp"
#include "entities/mainCharacter.hpp"
#include "worlds/startLocation.hpp"
#include <algorithm>

// the data
namespace {
    int playerHealth = 75;
    int playerMaxHealth = 100;
    std::shared_ptr<Entities::Character> playerCharacter;
    std::shared_ptr<Worlds::World> world;
    std::shared_ptr<UI> ui;
    SDL_Renderer *renderer;
    int screenWidth = 1280;
    int screenHeight = 720;
    int mouseX = 0;
    int mouseY = 0;
    float focusX = 0;
    float focusY = 0;
    bool rerender = true;
    TTF_Font *font = nullptr;
}

void GlobalsInit() {
    playerCharacter = std::make_shared<Entities::MainCharacter>();
    SetWorld(WorldId::StartLocation);
    ui = std::make_shared<UI>();
}

// player info
int GetPlayerHealth() {
    return playerHealth;
}
void SetPlayerHealth(int health) {
    playerHealth = health;
    std::clamp(playerHealth, 0, playerMaxHealth);
}
int GetPlayerMaxHealth() {
    return playerMaxHealth;
}
void SetPlayerMaxHealth(int maxHealth) {
    playerMaxHealth = maxHealth;
};
float GetPlayerHealthRatio() {
    return static_cast<float>(playerHealth) / playerMaxHealth;
}
std::shared_ptr<Entities::Character> GetPlayerCharacter() {
    return playerCharacter;
}

// game info
std::shared_ptr<Worlds::World> GetWorld() {
    return world;
}
void SetWorld(WorldId theWorld) {
    switch (theWorld) {
    case WorldId::StartLocation:
        world = std::make_shared<Worlds::StartLocation>();
        break;
    }
}

std::shared_ptr<UI> GetUI()
{
    return ui;
}

void SetMouse(int x, int y)
{
    mouseX = x;
    mouseY = y;

    const auto calculatedWidth = screenHeight * 16.0f / 9.0f;
    focusX = std::clamp(static_cast<float>(std::clamp(mouseX, static_cast<int>((screenWidth - calculatedWidth) / 2), static_cast<int>((screenWidth + calculatedWidth) / 2)) - static_cast<int>((screenWidth - calculatedWidth) / 2)) / calculatedWidth, 0.f, 1.f);
    focusY = static_cast<float>(std::clamp(mouseY, 0, screenHeight)) / screenHeight;
}
float GetFocusX()
{
    return focusX;
}
float GetFocusY()
{
    return focusY;
}

void SetRenderer(SDL_Renderer *theRenderer)
{
    renderer = theRenderer;
}

SDL_Renderer *GetRenderer()
{
    return renderer;
}

void SetScreenSize(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
}

int GetScreenWidth()
{
    return screenWidth;
}

int GetScreenHeight()
{
    return screenHeight;
}

int GetSceneWidth()
{
    return static_cast<int>(screenHeight * 16 / 9.f);
}

void SetRerender()
{
    rerender = true;
}
void UnsetRerender()
{
    rerender = false;
}
bool IsRerender()
{
    return rerender;
}

void SetFont(TTF_Font *theFont)
{
    font = theFont;
}
TTF_Font *GetFont()
{
    return font;
}
