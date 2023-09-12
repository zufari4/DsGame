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
            offsetX_ += (float)(event.motion.xrel);
            offsetY_ += (float)(event.motion.yrel);
        }
    } break;
    case SDL_MOUSEWHEEL: {
        setScale(scale_ + event.wheel.y * 0.5, event.wheel.mouseX, event.wheel.mouseY);
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
    return (screenX - offsetX_) / scale_;
}

float Camera::screenToWorldY(float screenY) const 
{
    return (screenY - offsetY_) / scale_;
}

float Camera::worldToScreenX(float worldX) const 
{
    return worldX * scale_ + offsetX_;
}

float Camera::worldToScreenY(float worldY) const 
{
    return worldY * scale_ + offsetY_;
}

void Camera::setScale(float s, float x, float y) 
{
    scale_ = s;
    offsetX_ = x;
    offsetY_ = y;
}
