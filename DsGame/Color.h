#pragma once

#include <inttypes.h>


struct Color
{
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    static const Color& red();
    static const Color& green();
    static const Color& blue();
    static const Color& white();
    static const Color& black();
};

