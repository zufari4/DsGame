#pragma once

#include <inttypes.h>

struct SDL_RWops;
struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Window;

SDL_RWops* getFontResourceData(int resID);
bool setColorKey(SDL_Surface* buffer, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB, bool flag);
SDL_Renderer* createPreferedRender(SDL_Window* window);