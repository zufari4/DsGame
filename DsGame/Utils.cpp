#include "Utils.h"
#include "resource.h"
#include <SDL2/SDL.h>
#include <windows.h>
#include <string>



SDL_RWops* getFontResourceData(int resID)
{
    HMODULE hModule = GetModuleHandle(nullptr);
    HRSRC hWhite = FindResource(hModule, MAKEINTRESOURCE(resID), RT_FONT);
    DWORD size = SizeofResource(hModule, hWhite);
    HGLOBAL hgWhite = LoadResource(hModule, hWhite);
    LPVOID pData = LockResource(hgWhite);
    return SDL_RWFromConstMem(pData, size);
}

SDL_bool SDL_BOOL(bool value)
{
    if (value)
    {
        return SDL_TRUE;
    }
    else
    {
        return SDL_FALSE;
    }
}

bool setColorKey(SDL_Surface* buffer, uint8_t colorkeyR, uint8_t colorkeyG, uint8_t colorkeyB, bool flag)
{
    uint32_t transparentColor = SDL_MapRGB(buffer->format, colorkeyR, colorkeyG, colorkeyB);
    return SDL_SetColorKey(buffer, SDL_BOOL(flag), transparentColor) == 0;
}

SDL_Renderer* createPreferedRender(SDL_Window* window, const std::vector<std::string>& driverNames)
{
    SDL_Renderer* renderer = nullptr;
    SDL_RendererInfo  rendererInfo;

    for (const auto& need : driverNames) {
        for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i) {
            SDL_GetRenderDriverInfo(i, &rendererInfo);
            if (rendererInfo.name == need) {
                return SDL_CreateRenderer(window, i, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            }
        }
    }

    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void getScreenSize(int& w, int& h)
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    w = DM.w;
    h = DM.h;
}

bool isRenderSupportMultiThreding(SDL_Renderer* renderer)
{
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    std::string driverName(info.name == nullptr ? "" : info.name);
    return driverName.find("direct3d") != std::string::npos;
}


