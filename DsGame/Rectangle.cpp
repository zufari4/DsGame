#include "Rectangle.h"
#include "Render.h"
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>
#include <stdio.h>


#define PUSH_DISTANCE 0.05

Rectangle::Rectangle(cpSpace* space, Render& render, float x, float y, float w, float h)
    : space_(space)
    , body_(nullptr)
    , shape_(nullptr)
    , render_(render)
    , r_(0xF0)
    , g_(0xF0)
    , b_(0xF0)
    , a_(0xFF)
    , w_(w)
    , h_(h)
    , isStatic_(false)
{
    body_  = cpBodyNew(1.0f, cpMomentForBox(1.0f, w, h));
    cpBodySetPosition(body_, cpv(x, y));

    shape_ = cpBoxShapeNew(body_, w, h, PUSH_DISTANCE);
    cpShapeSetElasticity(shape_, 0.3f);
    cpShapeSetFriction(shape_, 0.5f);

    body_  = cpSpaceAddBody(space, body_);
    shape_ = cpSpaceAddShape(space, shape_);
    printf("ctor %p\n", this);
}

Rectangle::~Rectangle()
{
    printf("dtor %p\n", this);
    cpBodyRemoveShape(body_, shape_);
    cpSpaceRemoveShape(space_, shape_);
    if (!isStatic_) {
        cpSpaceRemoveBody(space_, body_);
    }
}

void Rectangle::draw()
{
    const cpSplittingPlane* planes = ((const cpPolyShape*)shape_)->planes;
    if (cpBodyIsSleeping(body_)) {
        render_.drawRectangleFilled(planes, 0, 0, 20, a_);
    }
    else {
        render_.drawRectangleFilled(planes, r_, g_, b_, a_);
    }
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

void Rectangle::setStatic()
{
    cpVect pos = cpBodyGetPosition(body_);

    cpSpaceRemoveShape(space_, shape_);
    cpSpaceRemoveBody(space_, body_);

    body_ = cpSpaceGetStaticBody(space_);
    cpBodySetPosition(body_, pos);

    shape_ = cpBoxShapeNew(body_, w_, h_, PUSH_DISTANCE);
    cpShapeSetElasticity(shape_, 0.3f);
    cpShapeSetFriction(shape_, 0.5f);

    shape_ = cpSpaceAddShape(space_, shape_);
    isStatic_ = true;
}

void Rectangle::setMass(float m)
{
    cpBodySetMass(body_, m);
}