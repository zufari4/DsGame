#include "MouseBody.h"
#include "Physics.h"
#include "Camera.h"
#include <SDL2/SDL.h>
#include <chipmunk/chipmunk.h>
#include <chipmunk/chipmunk_private.h>


#define GRABBABLE_MASK_BIT (1<<31)
cpShapeFilter GRAB_FILTER = { CP_NO_GROUP, GRABBABLE_MASK_BIT, GRABBABLE_MASK_BIT };
cpShapeFilter NOT_GRABBABLE_FILTER = { CP_NO_GROUP, ~GRABBABLE_MASK_BIT, ~GRABBABLE_MASK_BIT };


MouseBody::MouseBody(Physics& physics)
    : physics_(physics)
    , camera_(nullptr)
    , mouseBody_(cpBodyNewKinematic())
    , mouseJoint_(nullptr)
    , mousePntX_(0)
    , mousePntY_(0)
{

}

void MouseBody::handleEvents(const SDL_Event& event)
{
     if (event.type == SDL_MOUSEBUTTONDOWN) {
             const SDL_MouseButtonEvent& m = (const SDL_MouseButtonEvent&)(event);
             if (m.button == SDL_BUTTON_LEFT) {
                 physics_.pause();

                 cpSpace* space = physics_.getSpace();
                 cpFloat radius = 5.0;
                 cpPointQueryInfo info;
                 cpVect mouse_pnt = { mousePntX_, mousePntY_ };

                 cpShape* shape = cpSpacePointQueryNearest(space, mouse_pnt, radius, GRAB_FILTER, &info);

                 if (shape && cpBodyGetMass(cpShapeGetBody(shape)) < INFINITY) {
                     // Use the closest point on the surface if the click is outside of the shape.
                     cpVect nearest = (info.distance > 0.0f ? info.point : mouse_pnt);
                     cpBody* body = cpShapeGetBody(shape);
                     mouseJoint_ = cpPivotJointNew2(mouseBody_, body, cpvzero, cpBodyWorldToLocal(body, nearest));
                     mouseJoint_->maxForce = 50000.0f;
                     mouseJoint_->errorBias = cpfpow(1.0f - 0.15f, 60.0f);
                     cpSpaceAddConstraint(space, mouseJoint_);
                 }

                 physics_.resume();
             }
     }
     else if (event.type == SDL_MOUSEBUTTONUP) {
         const SDL_MouseButtonEvent& m = (const SDL_MouseButtonEvent&)(event);
         if (m.button == SDL_BUTTON_LEFT) {
             physics_.pause();

             if (mouseJoint_) {
                 cpSpaceRemoveConstraint(physics_.getSpace(), mouseJoint_);
                 cpConstraintFree(mouseJoint_);
                 mouseJoint_ = nullptr;
             }

             physics_.resume();
         }
      }
     else if (event.type == SDL_MOUSEMOTION) {
         mousePntX_ = camera_->screenToWorldX(event.motion.x);
         mousePntY_ = camera_->screenToWorldY(event.motion.y);

         cpVect new_point = cpvlerp(mouseBody_->p, { mousePntX_, mousePntY_ }, 0.25f);
         mouseBody_->v = cpvmult(cpvsub(new_point, mouseBody_->p), 60.0f);
         mouseBody_->p = new_point;
     }
}

void MouseBody::setCamera(Camera& camera)
{
    camera_ = &camera;
}
