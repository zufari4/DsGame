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