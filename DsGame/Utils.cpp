#include "Utils.h"
#include "resource.h"
#include <SDL2/SDL.h>
#include <windows.h>


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