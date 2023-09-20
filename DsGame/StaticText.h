#pragma once

#include <memory>

struct SDL_Renderer;
struct SDL_Texture;
struct SDL_FRect;


class StaticText
{
public:
    StaticText(SDL_Renderer* renderer, const char* text, float size, float x, float y);
    ~StaticText();
    void draw();
private:
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    std::unique_ptr<SDL_FRect> rect_;
};