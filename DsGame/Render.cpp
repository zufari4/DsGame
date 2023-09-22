#include "Render.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include <SDL2/SDL_render.h>
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>


Render::Render(SDL_Renderer* renderer, Camera& camera, uint8_t clearColorR, uint8_t clearColorG, uint8_t clearColorB)
    : renderer_(renderer)
    , camera_(camera)
    , clearColorR_(clearColorR)
    , clearColorG_(clearColorG)
    , clearColorB_(clearColorB)
    , vertexBuffer_(std::make_unique<VertexBuffer>(1000))
{

}

void Render::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(renderer_, r, g, b, a);
}

void Render::drawLine(float x1, float y1, float x2, float y2) 
{
    SDL_RenderDrawLineF(renderer_,
        camera_.worldToScreenX(x1), camera_.worldToScreenY(y1),
        camera_.worldToScreenX(x2), camera_.worldToScreenY(y2)
    );
}

void Render::drawPoint(float x, float y)
{
    SDL_RenderDrawPoint(renderer_, camera_.worldToScreenX(x), camera_.worldToScreenY(y));
}

void Render::drawRectangleFilled(const cpSplittingPlane* planes, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    for (size_t i = 0; i < 4; ++i) {
        vertexBuffer_->setVertex(i, 
            camera_.worldToScreenX(planes[i].v0.x), 
            camera_.worldToScreenY(planes[i].v0.y),
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
        camera_.worldToScreenX(centerX),
        camera_.worldToScreenY(centerY),
        r, g, b, a
    );
    for (size_t i = 0; i < xShape.size(); ++i) {
        vertexBuffer_->setVertex(i+1,
            camera_.worldToScreenX(centerX + xShape[i]),
            camera_.worldToScreenY(centerY + yShape[i]),
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

SDL_Renderer* Render::getRender()
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
