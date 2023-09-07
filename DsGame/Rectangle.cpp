#include "Rectangle.h"
#include "Camera.h"
#include <SDL2/SDL.h>
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>



Rectangle::Rectangle(cpSpace* space, SDL_Renderer* screen, float x, float y, float w, float h)
    : body_(nullptr)
    , shape_(nullptr)
    , screen_(screen)
    , r_(0xF0)
    , g_(0xF0)
    , b_(0xF0)
    , a_(0xFF)
    , camera_(nullptr)
{
    body_  = cpBodyNew(1.0f, cpMomentForBox(1.0f, w, h));
    cpBodySetPosition(body_, cpv(x, y));

    shape_ = cpBoxShapeNew(body_, w, h, 0.5f);
    cpShapeSetElasticity(shape_, 0.3f);
    cpShapeSetFriction(shape_, 0.5f);

    body_  = cpSpaceAddBody(space, body_);
    shape_ = cpSpaceAddShape(space, shape_);
}

void Rectangle::draw()
{
    const cpSplittingPlane* planes = ((const cpPolyShape*)shape_)->planes;
    float offset_x = camera_ != nullptr ? camera_->getOffsetX() : 0;
    float offset_y = camera_ != nullptr ? camera_->getOffsetY() : 0;

    SDL_SetRenderDrawColor(screen_, r_, g_, b_, a_);

    SDL_RenderDrawLineF(screen_, planes[0].v0.x + offset_x, planes[0].v0.y + offset_y, planes[1].v0.x + offset_x, planes[1].v0.y + offset_y);
    SDL_RenderDrawLineF(screen_, planes[1].v0.x + offset_x, planes[1].v0.y + offset_y, planes[2].v0.x + offset_x, planes[2].v0.y + offset_y);
    SDL_RenderDrawLineF(screen_, planes[2].v0.x + offset_x, planes[2].v0.y + offset_y, planes[3].v0.x + offset_x, planes[3].v0.y + offset_y);
    SDL_RenderDrawLineF(screen_, planes[3].v0.x + offset_x, planes[3].v0.y + offset_y, planes[0].v0.x + offset_x, planes[0].v0.y + offset_y);
}

void Rectangle::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    r_ = r;
    g_ = g;
    b_ = b;
    a_ = a;
}

void Rectangle::setPos(float x, float y)
{
    cpBodySetPosition(body_, cpv(x, y));
}

void Rectangle::setAngle(float a)
{
    cpBodySetAngle(body_, a);
}

void Rectangle::resetVelocity()
{
    cpBodySetVelocity(body_, cpvzero);
    cpBodySetAngularVelocity(body_, 0);
    //cpBodySetForce(body_, cpvzero);
    //cpBodySetMoment(body_, 0);
    //cpBodySetTorque(body_, 0);
}

void Rectangle::setCamera(const Camera* camera) 
{
    camera_ = camera;
}
