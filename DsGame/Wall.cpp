#include "Wall.h"
#include "Physics.h"
#include <chipmunk/chipmunk.h>
#include <chipmunk/chipmunk_private.h>


Wall::Wall(Physics& physics, float x1, float y1, float x2, float y2, float radius)
    : physics_(physics)
{
    body_  = cpSpaceGetStaticBody(physics.getSpace());
    shape_ = cpSpaceAddShape(physics.getSpace(), cpSegmentShapeNew(body_, cpv(x1, y1), cpv(x2, y2), radius));
    cpShapeSetElasticity(shape_, 1.0f);
    cpShapeSetFriction(shape_, 1.0f);
}

Wall::~Wall()
{
    cpBodyRemoveShape(body_, shape_);
    cpSpaceRemoveShape(physics_.getSpace(), shape_);
}
