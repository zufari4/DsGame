#pragma once

struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Texture;

class IFontAtlas
{
public:
    IFontAtlas() = default;
    virtual ~IFontAtlas() = default;
    virtual bool build(SDL_Renderer* renderer, void* ttfFont) = 0;
    virtual const SDL_Rect* getGlyphRect(char c) const = 0;
    virtual int getGlyphAdvice(char c) const = 0;
    virtual SDL_Texture* getTexture() const = 0;
};