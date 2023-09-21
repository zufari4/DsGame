#include "FontAtlas.h"
#include <SDL2/SDL_ttf.h>
#include "Utils.h"


bool FontAtlas::build(SDL_Renderer* renderer, void* ttfFont)
{
    TTF_Font* font = (TTF_Font*)ttfFont;
    uint16_t ascii_char;                // Integer type expected by SDL2_ttf
    const uint32_t starting_glyph = 32; // Space character
    const uint32_t ending_glyph = 127;  // Tilde character
    const uint32_t fontHeight = TTF_FontHeight(font);

    // Glyph metrics
    int advance = 0;      // Spacing between characters
    int glyph_width = 0;  // Glyph's width in pixels
    int glyph_height = 0; // Glyph's height in pixels

    // Texture sheet calculations
    int padding = 1;
    int spacing = 2;

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    uint32_t pixel_format = info.texture_formats[0];

    int bpp = 0; // bits per pixel
    uint32_t red_mask = 0;
    uint32_t green_mask = 0;
    uint32_t blue_mask = 0;
    uint32_t alpha_mask = 0;

    // Find the best surface format based on the requested pixel_format
    SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &red_mask, &green_mask, &blue_mask, &alpha_mask);
    auto sheet = SDL_CreateRGBSurface(0, 16 * 16, 16 * 16, bpp, red_mask, green_mask, blue_mask, alpha_mask);

    // ASCII 32..127 is the standard glyph set -- 94 printable characters;
    //
    // 32 = ' ' (space)
    // 65 = 'A'
    // 122 = 'z'
    // 126 = '~' (tilde)

    uint32_t offsetX = 0;
    uint32_t offsetY = 0;
    SDL_Rect blit;

    for (uint32_t glyph = starting_glyph; glyph < ending_glyph; ++glyph) {
        ascii_char = static_cast<uint16_t> (glyph);

        if (!TTF_GlyphIsProvided(font, ascii_char)) {
            continue;
        }
        // We obtain width & height of a glyph from its rendered form
        auto glyph_image_orig = TTF_RenderGlyph32_LCD(font, ascii_char, { 200, 200, 200, 255 }, { 31, 31, 31, 255 });
        auto glyph_image = SDL_ConvertSurfaceFormat(glyph_image_orig, pixel_format, 0);
        SDL_FreeSurface(glyph_image_orig);

        glyph_width = glyph_image->w;
        glyph_height = glyph_image->h;

        // -_-
        // Disappointedly, the only metric that we can use here is the advance
        int ret = TTF_GlyphMetrics(font,
            ascii_char,
            nullptr, // Left (X) origin
            nullptr, // Width
            nullptr, // Top (Y) origin
            nullptr, // Height
            &advance
        );

        int bouce_w = glyph_width + spacing * padding;
        int bouce_h = glyph_height + spacing * padding;

        if (offsetX + bouce_w > sheet->w) {
            offsetY += fontHeight; // TODO: Add y overflow
            offsetX = 0;
        }

        glyphs_[glyph].advance = advance;
        glyphs_[glyph].bounds.x = offsetX;
        glyphs_[glyph].bounds.y = offsetY;
        glyphs_[glyph].bounds.w = bouce_w;
        glyphs_[glyph].bounds.h = bouce_h;

        offsetX += bouce_w;

        // Prepare the coordinates for rendering a glyph onto our texture sheet
        // we are creating.
        blit.x = glyphs_[glyph].bounds.x;
        blit.y = glyphs_[glyph].bounds.y;
        blit.w = -1; // Why -1 ???
        blit.h = -1; // Why -1 ???

        SDL_BlitSurface(glyph_image, nullptr, sheet, &blit);
    }

    // Turn color key transparency on so we are not left with a black,
    // AKA non-transparent background.
    setColorKey(sheet, 31, 31, 31, true);

    texture_ = SDL_CreateTextureFromSurface(renderer, sheet);
    w_ = sheet->w;
    h_ = sheet->h;
    SDL_FreeSurface(sheet);

    return true;
}

const SDL_Rect* FontAtlas::getGlyphRect(char c) const
{
    const auto& it = glyphs_.find((uint32_t)c);
    return it != glyphs_.end() ? &it->second.bounds : nullptr;
}

SDL_Texture* FontAtlas::getTexture() const
{
    return texture_;
}

int FontAtlas::getGlyphAdvice(char c) const
{
    const auto& it = glyphs_.find((uint32_t)c);
    return it != glyphs_.end() ? it->second.advance : 0;
}
