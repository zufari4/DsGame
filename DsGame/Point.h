#pragma once

struct Point
{
    float x;
    float y;
};

#pragma pack(push, 1) // for SDL_FPoint* cast

struct Point2
{
    Point2(float x1, float y1, float x2, float y2);
    Point a;
    Point b;
};

#pragma pack(pop)