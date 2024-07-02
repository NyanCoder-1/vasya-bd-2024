#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#else // __EMSCRIPTEN__
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif // __EMSCRIPTEN__
