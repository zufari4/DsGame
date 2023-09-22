#include "Grid.h"
#include "Render.h"
#include "SDL2/SDL_events.h"


Grid::Grid(Render& render, float size, const Color& color)
    : render_(render)
    , color_(color)
    , size_(size)
    , lines_(1000, {0,0,0,0})
    , countLines_(0)
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
    if (event.type == SDL_MOUSEWHEEL) {
        updateLines();
    }
}

void Grid::updateLines()
{
    int w, h;
    render_.getWindowSize(w, h);
    float scale = render_.getScale() / 100;
    float stepSize = size_ * scale;
    countLines_ = 0;

    bool revert = false;
    for (float x = stepSize; x < w && countLines_ < lines_.size(); x += stepSize) {
        if (revert) {
            lines_[countLines_++] = { x, 0, x, (float)h };
        }
        else {
            lines_[countLines_++] = { x, (float)h, x, 0 };
        }
        revert = !revert;
    }

    revert = false;
    for (float y = stepSize; y < h && countLines_ < lines_.size(); y += stepSize) {
        if (revert) {
            lines_[countLines_++] = { 0, y, (float)w, y };
        }
        else {
            lines_[countLines_++] = { (float)w, y, 0, y };
        }
        revert = !revert;
    }
}
