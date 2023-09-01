#include "Rectangle.h"
#include <SDL2/SDL.h>
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>



Rectangle::Rectangle(cpSpace* space, SDL_Renderer* screen, float x, float y, float w, float h)
    : body_(nullptr)
    , shape_(nullptr)
    , screen_(screen)
{
    body_  = cpBodyNew(1.0f, cpMomentForBox(1.0f, w, h));
    cpBodySetPosition(body_, cpv(x, y));

    shape_ = cpBoxShapeNew(body_, w, h, 0.5f);
    cpShapeSetElasticity(shape_, 0.3f);
    cpShapeSetFriction(shape_, 0.5f);

    body_  = cpSpaceAddBody(space, body_);
    shape_ = cpSpaceAddShape(space, shape_);
}

void Rectangle::draw() const
{
    const cpSplittingPlane* planes = ((const cpPolyShape*)shape_)->planes;

    SDL_SetRenderDrawColor(screen_, 0xFA, 0xFA, 0xFA, 0xFF);
    SDL_RenderDrawLineF(screen_, planes[0].v0.x, planes[0].v0.y, planes[1].v0.x, planes[1].v0.y);
    SDL_RenderDrawLineF(screen_, planes[1].v0.x, planes[1].v0.y, planes[2].v0.x, planes[2].v0.y);
    SDL_RenderDrawLineF(screen_, planes[2].v0.x, planes[2].v0.y, planes[3].v0.x, planes[3].v0.y);
    SDL_RenderDrawLineF(screen_, planes[3].v0.x, planes[3].v0.y, planes[0].v0.x, planes[0].v0.y);
}
