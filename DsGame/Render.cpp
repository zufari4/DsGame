#include "Render.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "Utils.h"
#include <SDL2/SDL.h>
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>


Render::Render(uint8_t clearColorR, uint8_t clearColorG, uint8_t clearColorB)
    : renderer_(nullptr)
    , window_(nullptr)
    , camera_(nullptr)
    , clearColorR_(clearColorR)
    , clearColorG_(clearColorG)
    , clearColorB_(clearColorB)
    , vertexBuffer_(std::make_unique<VertexBuffer>(1000))
    , isSupportMultiThreding_(false)
{

}

bool Render::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    int w, h;
    getScreenSize(w, h);
    window_ = SDL_CreateWindow("DS Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    renderer_ = createPreferedRender(window_, { "direct3d11", "opengl" });
    isSupportMultiThreding_ = isRenderSupportMultiThreding(renderer_);
    return renderer_ != nullptr;
}

void Render::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void Render::setDrawColor(const Color& color)
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

void Render::drawLine(float x1, float y1, float x2, float y2) 
{
    SDL_RenderDrawLineF(renderer_,
        camera_->worldToScreenX(x1), camera_->worldToScreenY(y1),
        camera_->worldToScreenX(x2), camera_->worldToScreenY(y2)
    );
}

void Render::drawLineNoScale(const Point2& points)
{
    SDL_RenderDrawLinesF(renderer_, (const SDL_FPoint*)(&points), 2);
    //SDL_RenderDrawLineF(renderer_, points.a.x, points.a.y, points.b.x, points.b.y);
}

void Render::drawLinesNoScale(const Point2* points, int count)
{
    SDL_RenderDrawLinesF(renderer_, (const SDL_FPoint*)points, count);
}

void Render::drawPoint(float x, float y)
{
    SDL_RenderDrawPoint(renderer_, camera_->worldToScreenX(x), camera_->worldToScreenY(y));
}

void Render::drawRectangleFilled(const cpSplittingPlane* planes, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    for (size_t i = 0; i < 4; ++i) {
        vertexBuffer_->setVertex(i, 
            camera_->worldToScreenX(planes[i].v0.x), 
            camera_->worldToScreenY(planes[i].v0.y),
            r, g, b, a
        );
    }
    vertexBuffer_->setIndex(0, 0);
    vertexBuffer_->setIndex(1, 1);
    vertexBuffer_->setIndex(2, 3);
    vertexBuffer_->setIndex(3, 3);
    vertexBuffer_->setIndex(4, 2);
    vertexBuffer_->setIndex(5, 1);

    SDL_RenderGeometry(renderer_, nullptr, vertexBuffer_->getVertexes(), 4, vertexBuffer_->getIndexes(), 6);
}

void Render::drawCircleFilled(float centerX, float centerY, const std::vector<float>& xShape, const std::vector<float>& yShape, 
    uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    vertexBuffer_->setVertex(0,
        camera_->worldToScreenX(centerX),
        camera_->worldToScreenY(centerY),
        r, g, b, a
    );
    for (size_t i = 0; i < xShape.size(); ++i) {
        vertexBuffer_->setVertex(i+1,
            camera_->worldToScreenX(centerX + xShape[i]),
            camera_->worldToScreenY(centerY + yShape[i]),
            r, g, b, a
        );
    }

    size_t countIndexes = 0;

    for (size_t i = 0; i < xShape.size(); ++i) {
        vertexBuffer_->setIndex(countIndexes++, 0);
        vertexBuffer_->setIndex(countIndexes++, i);
        vertexBuffer_->setIndex(countIndexes++, i+1);
    }
    vertexBuffer_->setIndex(countIndexes++, 0);
    vertexBuffer_->setIndex(countIndexes++, 1);
    vertexBuffer_->setIndex(countIndexes++, xShape.size());

    SDL_RenderGeometry(renderer_, nullptr, vertexBuffer_->getVertexes(), xShape.size()+1, vertexBuffer_->getIndexes(), countIndexes);
}

SDL_Renderer* Render::getRenderer()
{
    return renderer_;
}

void Render::clear()
{
    SDL_SetRenderDrawColor(renderer_, clearColorR_, clearColorG_, clearColorB_, 255);
    SDL_RenderClear(renderer_);
}

void Render::present()
{
    SDL_RenderPresent(renderer_);
}

bool Render::isSupportMultiThreding() const
{
    return isSupportMultiThreding_;
}

void Render::getWindowSize(int& w, int& h) const
{
    auto win = SDL_RenderGetWindow(renderer_);
    SDL_GetWindowSize(win, &w, &h);
}

float Render::getScale()
{
    return camera_->getScale();
}

float Render::getCameraOffsetX()
{
    return camera_->getOffsetX();
}

float Render::getCameraOffsetY()
{
    return camera_->getOffsetY();
}

void Render::setCamera(const Camera& camera)
{
    camera_ = &camera;
}

void Render::free()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}
