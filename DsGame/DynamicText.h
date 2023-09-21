#pragma once

#include "IFontAtlas.h"
#include <string>
#include <memory>


struct SDL_Renderer;


class DynamicText
{
public:
    DynamicText(SDL_Renderer* renderer, float x, float y, float size);
    void draw(const std::string& str);
    void draw(const char* format, ...);
private:
    std::unique_ptr<IFontAtlas> createAtlas(SDL_Renderer* renderer, void* font);
    void drawImpl(const char* cStr, size_t len);

    SDL_Renderer* renderer_;
    float x_;
    float y_;
    std::unique_ptr<IFontAtlas> atlas_;
    char buff_[65535];
};