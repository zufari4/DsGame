#pragma once

#include <inttypes.h>
#include <string>
#include <vector>
#include <random>

struct SDL_RWops;
struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Window;

SDL_RWops* getFontResourceData(int resID);
bool setColorKey(SDL_Surface* buffer, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB, bool flag);
SDL_Renderer* createPreferedRender(SDL_Window* window, const std::vector<std::string>& driverNames);
void getScreenSize(int& w, int& h);
bool isRenderSupportMultiThreding(SDL_Renderer* renderer);

template<typename t>
t rnd(t start, t end)
{
    static std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<t> uni(start, end);

    return uni(rd);
}