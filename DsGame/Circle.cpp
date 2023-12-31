#include "Circle.h"
#include "Render.h"
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>


Cirle::Cirle(cpSpace* space, Render& render, float x, float y, float r)
    : space_(space)
    , body_(nullptr)
    , shape_(nullptr)
    , render_(render)
    , r_(0xf0)
    , g_(0xf0)
    , b_(0xf0)
    , a_(0xff)
    , radius_(r)
    , isStatic_(false)
{
    body_ = cpBodyNew(1.0f, cpMomentForCircle(1.0f, 0, r, cpvzero));
    cpBodySetPosition(body_, cpv(x, y));

    shape_ = cpCircleShapeNew(body_, r, cpvzero);
    cpShapeSetElasticity(shape_, 0.7f);
    cpShapeSetFriction(shape_, 0.5f);

    body_  = cpSpaceAddBody(space, body_);
    shape_ = cpSpaceAddShape(space, shape_);

    updateDrawShape(r, 16);
    printf("ctor %p\n", this);
}

Cirle::~Cirle()
{
    printf("dtor %p\n", this);
    cpBodyRemoveShape(body_, shape_);
    cpSpaceRemoveShape(space_, shape_);
    if (!isStatic_) {
        cpSpaceRemoveBody(space_, body_);
    }
}

void Cirle::draw()
{
    const cpCircleShape* shape = ((const cpCircleShape*)shape_);
    const cpVect& pos = shape->tc;

    render_.drawCircleFilled(pos.x, pos.y, xShape_, yShape_, r_, g_, b_, a_);

    render_.setDrawColor(r_/2, g_/2, b_/2, a_);
    const cpVect& rv = cpvadd(pos, cpvmult(cpvforangle(body_->a), shape->r));
    render_.drawLine(pos.x, pos.y, rv.x, rv.y);
}

void Cirle::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    r_ = r;
    g_ = g;
    b_ = b;
    a_ = a;
}

void Cirle::setPos(float x, float y)
{
    cpBodySetPosition(body_, cpv(x, y));
}

void Cirle::setAngle(float a)
{
    cpBodySetAngle(body_, a);
}

void Cirle::resetVelocity()
{
    cpBodySetVelocity(body_, cpvzero);
    cpBodySetAngularVelocity(body_, 0);
    //cpBodySetForce(body_, cpvzero);
    //cpBodySetMoment(body_, 0);
    //cpBodySetTorque(body_, 0);
}

void Cirle::setStatic()
{
    cpVect pos = cpBodyGetPosition(body_);

    cpSpaceRemoveShape(space_, shape_);
    cpSpaceRemoveBody(space_, body_);

    body_ = cpSpaceGetStaticBody(space_);
    cpBodySetPosition(body_, pos);

    shape_ = cpCircleShapeNew(body_, radius_, cpvzero);
    cpShapeSetElasticity(shape_, 0.6f);
    cpShapeSetFriction(shape_, 0.7f);

    shape_ = cpSpaceAddShape(space_, shape_);

    isStatic_ = true;
}

void Cirle::setMass(float m)
{
    cpBodySetMass(body_, m);
}

void Cirle::updateDrawShape(double radius, int numSegments)
{
    xShape_.clear();
    yShape_.clear();

    double pi2 = 3.14159265358979323846264338327950288419716939937510 * 2.0;
    for (int ii = 0; ii < numSegments; ii++) {
        double theta = pi2 * double(ii) / double(numSegments);
        xShape_.push_back(radius * cos(theta));
        yShape_.push_back(radius * sin(theta));
    }

}
