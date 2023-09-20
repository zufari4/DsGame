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
{
    body_ = cpBodyNew(1.0f, cpMomentForCircle(1.0f, 0, r, cpvzero));
    cpBodySetPosition(body_, cpv(x, y));

    shape_ = cpCircleShapeNew(body_, r, cpvzero);
    cpShapeSetElasticity(shape_, 0.6f);
    cpShapeSetFriction(shape_, 0.7f);

    body_  = cpSpaceAddBody(space, body_);
    shape_ = cpSpaceAddShape(space, shape_);

    updateDrawShape(r, r);
}

void Cirle::draw()
{
    const cpCircleShape* shape = ((const cpCircleShape*)shape_);
    const cpVect& pos = shape->tc;

    render_.setDrawColor(r_, g_, b_, a_);

    for (size_t i = 0; i < xShape_.size() - 2; i += 2) {
        render_.drawLine( 
            pos.x + xShape_[i]  , pos.y + yShape_[i], 
            pos.x + xShape_[i+1], pos.y + yShape_[i+1]
        );
    }

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
}

void Cirle::updateDrawShape(float radiusX, float radiusY)
{
    const float x0 = 0;
    const float y0 = 0;

    xShape_.clear();
    yShape_.clear();

    float pi = 3.14159265358979323846264338327950288419716939937510;
    float pih = pi / 2.0; //half of pi

    //drew  28 lines with   4x4  circle with precision of 150 0ms
    //drew 132 lines with  25x14 circle with precision of 150 0ms
    //drew 152 lines with 100x50 circle with precision of 150 3ms
    const float prec = 5; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0;     // angle that will be increased each loop

    //starting point
    float x = radiusX * cosf(theta);//start point
    float y = radiusY * sinf(theta);//start point
    float x1 = x;
    float y1 = y;

    //repeat until theta >= 90;
    float step = pih / prec; // amount to add to theta each time (degrees)
    for (theta = step; theta <= pih; theta += step)//step through only a 90 arc (1 quadrant)
    {
        //get new point location
        x1 = radiusX * cosf(theta) + 0.5; //new point (+.5 is a quick rounding method)
        y1 = radiusY * sinf(theta) + 0.5; //new point (+.5 is a quick rounding method)

        //draw line from previous point to new point, ONLY if point incremented
        if ((x != x1) || (y != y1))//only draw if coordinate changed
        {
            xShape_.push_back(x0 + x); yShape_.push_back(y0 - y); xShape_.push_back(x0 + x1); yShape_.push_back(y0 - y1);//quadrant TR
            xShape_.push_back(x0 - x); yShape_.push_back(y0 - y); xShape_.push_back(x0 - x1); yShape_.push_back(y0 - y1);//quadrant TL
            xShape_.push_back(x0 - x); yShape_.push_back(y0 + y); xShape_.push_back(x0 - x1); yShape_.push_back(y0 + y1);//quadrant BL
            xShape_.push_back(x0 + x); yShape_.push_back(y0 + y); xShape_.push_back(x0 + x1); yShape_.push_back(y0 + y1);//quadrant BR
        }
        //save previous points
        x = x1;//save new previous point
        y = y1;//save new previous point
    }
    //arc did not finish because of rounding, so finish the arc
    if (x != 0)
    {
        x = 0;
        xShape_.push_back(x0 + x); yShape_.push_back(y0 - y); xShape_.push_back(x0 + x1); yShape_.push_back(y0 - y1);//quadrant TR
        xShape_.push_back(x0 - x); yShape_.push_back(y0 - y); xShape_.push_back(x0 - x1); yShape_.push_back(y0 - y1);//quadrant TL
        xShape_.push_back(x0 - x); yShape_.push_back(y0 + y); xShape_.push_back(x0 - x1); yShape_.push_back(y0 + y1);//quadrant BL
        xShape_.push_back(x0 + x); yShape_.push_back(y0 + y); xShape_.push_back(x0 + x1); yShape_.push_back(y0 + y1);//quadrant BR
    }
}
