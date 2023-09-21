#include "DynamicText.h"
#include "Utils.h"
#include "FontAtlas.h"
#include <SDL2/SDL_ttf.h>
#include <inttypes.h>
#include <map>
#include <stdarg.h>


DynamicText::DynamicText(SDL_Renderer* renderer, float x, float y, float size)
    : renderer_(renderer)
    , x_(x)
    , y_(y)
{
    TTF_Init();
    auto fontData = getFontResourceData(103);
    TTF_Font* font = TTF_OpenFontRW(fontData, 1, size);
    TTF_SetFontHinting(font, TTF_HINTING_LIGHT_SUBPIXEL);

    atlas_ = createAtlas(renderer, font);

    TTF_CloseFont(font);
}

void DynamicText::draw(const std::string& str)
{
    drawImpl(str.c_str(), str.length());
}

void DynamicText::draw(const char* format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    int sz = vsnprintf(buff_, sizeof(buff_), format, arglist);
    va_end(arglist);

    drawImpl(buff_, sz > 0 ? sz : 0);
}

std::unique_ptr<IFontAtlas> DynamicText::createAtlas(SDL_Renderer* renderer, void* fontPtr)
{
    std::unique_ptr<FontAtlas> res = std::make_unique<FontAtlas>();
    res->build(renderer, fontPtr);
    return res;
}

void DynamicText::drawImpl(const char* cStr, size_t len)
{
    FontAtlas* atlas = (FontAtlas*)(atlas_.get());

    int ofX = 0;
    SDL_Texture* map = atlas_->getTexture();
    SDL_Rect dstRect = { (int)x_, (int)y_, -1, -1 };

    const char* e = cStr + len;
    for (const char* c = cStr; c != e; ++c) {
        auto glyphRect = atlas_->getGlyphRect(*c);
        if (glyphRect == nullptr) continue;

        dstRect.w = glyphRect->w;
        dstRect.h = glyphRect->h;
        SDL_RenderCopy(renderer_, map, glyphRect, &dstRect);
        dstRect.x += atlas_->getGlyphAdvice(*c);
    }
}

