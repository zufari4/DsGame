#include "StaticText.h"
#include "Utils.h"
#include <SDL2/SDL_ttf.h>


StaticText::StaticText(SDL_Renderer* renderer, const char* text, float size, float x, float y)
    : renderer_(renderer)
    , texture_(nullptr)
{
    TTF_Init();
    auto fontData = getFontResourceData(103);
    TTF_Font* font = TTF_OpenFontRW(fontData, 1, size);
    SDL_Color fg = { 200, 200, 200 };

    auto surface = TTF_RenderText_Blended(font, text, fg);
    texture_ = SDL_CreateTextureFromSurface(renderer_, surface);

    rect_ = std::make_unique<SDL_FRect>();
    rect_->x = x;
    rect_->y = y;
    rect_->w = surface->w;
    rect_->h = surface->h;

    SDL_FreeSurface(surface);
}

StaticText::~StaticText()
{
    SDL_DestroyTexture(texture_);
}

void StaticText::draw()
{
    SDL_RenderCopyF(renderer_, texture_, NULL, rect_.get());
}
