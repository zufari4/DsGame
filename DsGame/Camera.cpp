#include "Camera.h"
#include <SDL2/SDL.h>


 Camera::Camera(SDL_Renderer* renderer) 
     : renderer_(renderer)
     , scale_(1.0)
     , offsetX_(0)
     , offsetY_(0)
     , dragStartX_(0)
     , dragStartY_(0)
     , dragIsStart_(false)
 {}

void Camera::handleEvent(const SDL_Event& event) 
{
    switch (event.type) {
    case SDL_MOUSEBUTTONDOWN: {
        if (event.button.button == SDL_BUTTON_MIDDLE) {
            dragStartX_ = event.button.x;
            dragStartY_ = event.button.y;
            dragIsStart_= true;
        }
    } break;
    case SDL_MOUSEBUTTONUP: {
        if (event.button.button == SDL_BUTTON_MIDDLE) {
            dragIsStart_= false;
        }
    } break;
    case SDL_MOUSEMOTION: {
        if (dragIsStart_) {
            float dx = (float)(event.motion.xrel)/scale_;
            float dy = (float)(event.motion.yrel)/scale_;
            offsetX_ += dx;
            offsetY_ += dy;
        }
    } break;
    case SDL_MOUSEWHEEL: {
        setScale(scale_ + event.wheel.y * 0.05);
    } break;
    }
 }

float Camera::getOffsetX() const
{
    return offsetX_;
}

float Camera::getOffsetY() const 
{
    return offsetY_;
}

void Camera::reset()
{
     scale_   = 1;
     offsetX_ = 0;
     offsetY_ = 0;
     dragStartX_  = 0;
     dragStartY_  = 0;
     dragIsStart_ = false;
}

float Camera::screenToWorldX(float screenX) const 
{
    return screenX/scale_ - offsetX_;
}

float Camera::screenToWorldY(float screenY) const 
{
    return screenY/scale_ - offsetY_;
}

void Camera::setScale(float s) 
{
    SDL_RenderSetScale(renderer_, s, s);
    scale_ = s;
}
