#pragma once

#include "IFontAtlas.h"
#include <SDL2/SDL.h>
#include <map>


class FontAtlas: public IFontAtlas
{
public:
    FontAtlas() = default;
    ~FontAtlas() = default;
    bool build(SDL_Renderer* renderer, void* ttfFont) override;
    const SDL_Rect* getGlyphRect(char c) const override;
    SDL_Texture* getTexture() const override;
    int getGlyphAdvice(char c) const override;
private:
    struct Glyph
    {
        SDL_Rect bounds;
        int advance;// Offset to move horizontally to the next character (spacing)
        SDL_Point offset = { 0,0 }; // The additional X, Y rendering offsets to apply when copying the
    };                              // glyph from the texture source to the output.
    using GlyphAtlas = std::map<uint32_t, Glyph>;
    GlyphAtlas   glyphs_;
    SDL_Texture* texture_;
    uint32_t w_;
    uint32_t h_;
};
