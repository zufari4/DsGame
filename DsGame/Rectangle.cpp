#include "Rectangle.h"
#include "Render.h"
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>



Rectangle::Rectangle(cpSpace* space, Render& render, float x, float y, float w, float h)
    : body_(nullptr)
    , shape_(nullptr)
    , render_(render)
    , r_(0xF0)
    , g_(0xF0)
    , b_(0xF0)
    , a_(0xFF)
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

    render_.setDrawColor(r_, g_, b_, a_);

    render_.drawLine(planes[0].v0.x, planes[0].v0.y, planes[1].v0.x, planes[1].v0.y);
    render_.drawLine(planes[1].v0.x, planes[1].v0.y, planes[2].v0.x, planes[2].v0.y);
    render_.drawLine(planes[2].v0.x, planes[2].v0.y, planes[3].v0.x, planes[3].v0.y);
    render_.drawLine(planes[3].v0.x, planes[3].v0.y, planes[0].v0.x, planes[0].v0.y);
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