#include "Camera.h"
#include "Render.h"
#include <SDL2/SDL_events.h>


 Camera::Camera(int screenW, int screenH, float scale)
     : scale_(scale)
     , offsetX_(0)
     , offsetY_(0)
     , dragStartX_(0)
     , dragStartY_(0)
     , dragIsStart_(false)
     , initScale_(scale)
 {
     setToCenterScreen(screenW, screenH);
 }

 void Camera::setPosition(float x, float y)
 {
     float deltaX = x - centerX_;
     float deltaY = y - centerY_;
     offsetX_ -= deltaX;
     offsetY_ -= deltaY;
     centerX_ = x;
     centerY_ = y;
 }

 void Camera::move(float dx, float dy)
 {
     offsetX_ += dx;
     offsetY_ += dy;
     centerX_ -= dx;
     centerY_ -= dy;
 }

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
            move(event.motion.xrel, event.motion.yrel);
        }
    } break;
    case SDL_MOUSEWHEEL: {
        setScale(scale_ + event.wheel.y * scale_*0.1, event.wheel.mouseX, event.wheel.mouseY);
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

void Camera::reset(int screenW, int screenH)
{
     offsetX_ = 0;
     offsetY_ = 0;
     dragStartX_  = 0;
     dragStartY_  = 0;
     dragIsStart_ = false;
     setToCenterScreen(screenW, screenH);
     scale_ = initScale_;
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
    float prevX = screenToWorldX(x);
    float prevY = screenToWorldY(y);

    scale_ = s;

    float newX = worldToScreenX(prevX);
    float newY = worldToScreenY(prevY);

    move(x - newX, y - newY);
}

void Camera::setScale(float value)
{
    setScale(value, centerX_, centerY_);
}

float Camera::getScale() const
{
    return scale_;
}

void Camera::setToCenterScreen(int screenW, int screenH)
{
    centerX_ = (float)screenW / 2.0;
    centerY_ = (float)screenH / 2.0;
}
