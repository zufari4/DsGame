#include "Grid.h"
#include "Render.h"
#include "SDL2/SDL_events.h"


Grid::Grid(Render& render, float size, const Color& color)
    : render_(render)
    , color_(color)
    , size_(size)
    , lines_(1000, {0,0,0,0})
    , countLines_(0)
    , dragStart_(false)
{
    updateLines();
}

void Grid::draw()
{
    render_.setDrawColor(color_);
    render_.drawLinesNoScale(lines_.data(), countLines_*2);
}

void Grid::handleEvents(const SDL_Event& event)
{
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
        if (event.button.button == SDL_BUTTON_MIDDLE) {
            dragStart_ = true;
        }
    } break;
    case SDL_MOUSEBUTTONUP: {
        if (event.button.button == SDL_BUTTON_MIDDLE) {
            dragStart_ = false;
        }
    } break;
    case SDL_MOUSEMOTION: {
        if (dragStart_) {
            updateLines();
        }
    } break;
    case SDL_MOUSEWHEEL: {
        updateLines();
    } break;
    }
}

void Grid::updateLines()
{
    int w, h;
    render_.getWindowSize(w, h);
    float scale = render_.getScale() / 100;
    float stepSize = size_ * scale;
    countLines_ = 0;
    float ox = 0;//(int((render_.getCameraOffsetX()) / stepSize)) * stepSize;
    float oy = 0;// (int((render_.getCameraOffsetY()) / stepSize))* stepSize;

    bool revert = false;
    for (float x = stepSize; x < w && countLines_ < lines_.size(); x += stepSize) {
        if (revert) {
            lines_[countLines_++] = { x + ox, 0 + oy, x + ox, (float)h + oy };
        }
        else {
            lines_[countLines_++] = { x + ox, (float)h + oy, x + ox, 0 + oy };
        }
        revert = !revert;
    }

    revert = false;
    for (float y = stepSize; y < h && countLines_ < lines_.size(); y += stepSize) {
        if (revert) {
            lines_[countLines_++] = { 0 + ox, y + oy, (float)w + ox, y + oy };
        }
        else {
            lines_[countLines_++] = { (float)w + ox, y + oy, 0 + ox, y + oy };
        }
        revert = !revert;
    }
}
