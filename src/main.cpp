#include "emsdl.hpp"
#include "globals.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

bool quit = false; // Main loop flag
SDL_Window *window;
SDL_Renderer *renderer;

std::chrono::high_resolution_clock::time_point previousTime;
float fakeHealthRatio = 0.75f;

void cleanup()
{
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void mainLoop()
{
    // Event handler
    SDL_Event e;
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                if (e.window.data1 > 20 && e.window.data2 > 20)
                {
                    SDL_RenderSetViewport(renderer, NULL);
                    SetScreenSize(e.window.data1, e.window.data2);
                }
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            SetMouse(e.motion.x, e.motion.y);
            break;
        }
    }

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsed = currentTime - previousTime;
    previousTime = currentTime;
    float dtime = std::chrono::duration_cast<std::chrono::duration<float>>(elapsed).count();

    fakeHealthRatio = std::fmodf(fakeHealthRatio + dtime, 1.0f);

    GetWorld()->update(dtime);
    GetUI()->update(dtime);

    SDL_SetRenderDrawColor(renderer, 12, 12, 0, 255);
    SDL_RenderClear(renderer);
    GetWorld()->render();
    GetUI()->render();

    SDL_SetRenderDrawColor(renderer, 12, 12, 0, 255);
    SDL_Rect rects[2] = {
        { .x = -1000, .y = 0, .w = static_cast<int>(GetScreenWidth() - GetSceneWidth()) / 2 + 1000, .h = GetScreenHeight() },
        { .x = static_cast<int>(GetScreenWidth() + GetSceneWidth()) / 2, .y = 0, .w = static_cast<int>(GetScreenWidth() - GetSceneWidth()) / 2 + 1000, .h = GetScreenHeight() }
    };
    SDL_RenderFillRects(renderer, rects, 2);

    // SDL_Point focusScreen{ static_cast<int>(GetFocusX() * GetSceneWidth()) + static_cast<int>((GetScreenWidth() - GetSceneWidth()) / 2), static_cast<int>(GetFocusY() * GetScreenHeight()) };
    // SDL_Rect rect{focusScreen.x - 2, focusScreen.y - 2, 4, 4};
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    // SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1280, 720, SDL_WINDOW_RESIZABLE, &window, &renderer);
    SetRenderer(renderer);

    SDL_SetRenderDrawColor(renderer, 0x3f, 0x3f, 0, 255);
    if (TTF_Init())
    {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    {
        auto fontPath = std::filesystem::current_path()/"assets"/"unifont-15.1.05.otf";
        TTF_Font *font = TTF_OpenFont(fontPath.c_str(), 45);
        if (!font) {
            std::cerr << "Couldn't open ttf font file: " << fontPath.c_str() << " with error \"" << TTF_GetError() << "\"" << std::endl;
            exit(EXIT_FAILURE);
        }
        SetFont(font);
    }
    if (!IMG_Init(IMG_INIT_PNG))
    {
        std::cerr << "IMG_Init: " << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    GlobalsInit();

    previousTime = std::chrono::high_resolution_clock::now();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, 1);
#else  // __EMSCRIPTEN__

    // While application is running
    while (!quit)
    {
        mainLoop();
    }

    cleanup();
#endif // __EMSCRIPTEN__

    return 0;
}