#pragma once

#include "Point.h"
#include "Color.h"
#include <vector>


class Render;
union SDL_Event;


class Grid
{
public:
    Grid(Render& render, float size, const Color& color);
    void draw();
    void handleEvents(const SDL_Event& event);
private:
    void updateLines();

    Render& render_;
    Color color_;
    float size_;
    std::vector<Point2> lines_;
    size_t countLines_;
    bool dragStart_;
};

