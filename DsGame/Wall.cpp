#include "Wall.h"
#include "Physics.h"
#include <chipmunk/chipmunk.h>


Wall::Wall(Physics& physics, float x1, float y1, float x2, float y2, float radius)
{
    cpBody* staticBody = cpSpaceGetStaticBody(physics.getSpace());
    cpShape* shape = cpSpaceAddShape(physics.getSpace(), cpSegmentShapeNew(staticBody, cpv(x1, y1), cpv(x2, y2), radius));
    cpShapeSetElasticity(shape, 1.0f);
    cpShapeSetFriction(shape, 1.0f);
}
