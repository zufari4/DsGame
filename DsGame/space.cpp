#include "space.h"
#include <chipmunk/cpHastySpace.h>

cpShapeFilter GRAB_FILTER = {CP_NO_GROUP, GRABBABLE_MASK_BIT, GRABBABLE_MASK_BIT};
cpShapeFilter NOT_GRABBABLE_FILTER = {CP_NO_GROUP, ~GRABBABLE_MASK_BIT, ~GRABBABLE_MASK_BIT};

static cpBody  *mouse_body       = NULL;
static cpConstraint *mouse_joint = NULL;
static cpVect mouse_pnt;

void space_init(cpSpace* space, int width, int height)
{ 
  cpBody* staticBody = cpSpaceGetStaticBody(space);
  cpShape *shape;
  
  // Create segments around the edge of the screen.
  shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0,-height), cpv(0,height), 0.0f));
  cpShapeSetElasticity(shape, 1.0f);
  cpShapeSetFriction(shape, 1.0f);

  shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(width,-height), cpv(width,height), 0.0f));
  cpShapeSetElasticity(shape, 1.0f);
  cpShapeSetFriction(shape, 1.0f);

  shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0,height), cpv(width,height), 0.0f));
  cpShapeSetElasticity(shape, 1.0f);
  cpShapeSetFriction(shape, 1.0f);
   
  mouse_body = cpBodyNewKinematic();
}

void
space_update() {
    cpVect new_point = cpvlerp(mouse_body->p, mouse_pnt, 0.25f);
    mouse_body->v = cpvmult(cpvsub(new_point, mouse_body->p), 60.0f);
    mouse_body->p = new_point;
}

// EVENTS
void
space_mouse_down(cpSpace* space) {

  // give the mouse click a little radius to make it easier to click small shapes.
  cpFloat radius = 5.0;
  
  cpPointQueryInfo info;
  cpShape *shape = cpSpacePointQueryNearest(space, mouse_pnt, radius, GRAB_FILTER, &info);
  
  if(shape && cpBodyGetMass(cpShapeGetBody(shape)) < INFINITY){
    // Use the closest point on the surface if the click is outside of the shape.
    cpVect nearest = (info.distance > 0.0f ? info.point : mouse_pnt);
    
    cpBody *body = cpShapeGetBody(shape);
    mouse_joint = cpPivotJointNew2(mouse_body, body, cpvzero, cpBodyWorldToLocal(body, nearest));
    mouse_joint->maxForce = 50000.0f;
    mouse_joint->errorBias = cpfpow(1.0f - 0.15f, 60.0f);
    cpSpaceAddConstraint(space, mouse_joint);
  }
}

void
space_mouse_up(cpSpace* space) {
  if (mouse_joint) {
    cpSpaceRemoveConstraint(space, mouse_joint);
    cpConstraintFree(mouse_joint);
    mouse_joint = NULL;
  }
}

void
space_mouse_move(cpSpace* space, int x, int y) {
  mouse_pnt.x = x;
  mouse_pnt.y = y;
}