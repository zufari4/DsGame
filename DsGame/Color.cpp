#include "Color.h"

Color::Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a /*= 255*/)
    : r(_r), g(_g), b(_b), a(_a)
{

}

const Color& Color::red()
{
    static const Color c { 255, 0, 0 };
    return c;
}

const Color& Color::green()
{
    static const Color c{ 0, 255, 0 };
    return c;
}

const Color& Color::blue()
{
    static const Color c{ 0, 0, 255 };
    return c;
}

const Color& Color::white()
{
    static const Color c{ 255, 255, 255 };
    return c;
}

const Color& Color::black()
{
    static const Color c{ 0, 0, 0 };
    return c;
}
