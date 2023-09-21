#include "Camera.h"
#include <SDL2/SDL.h>


 Camera::Camera(SDL_Renderer* renderer, float scale)
     : renderer_(renderer)
     , scale_(scale)
     , offsetX_(0)
     , offsetY_(0)
     , dragStartX_(0)
     , dragStartY_(0)
     , dragIsStart_(false)
     , initScale_(scale)
 {
     setToCenterScreen();
     //setScale(scale);
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
        setScale(scale_ + event.wheel.y * 2, event.wheel.mouseX, event.wheel.mouseY);
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
     offsetX_ = 0;
     offsetY_ = 0;
     dragStartX_  = 0;
     dragStartY_  = 0;
     dragIsStart_ = false;
     setToCenterScreen();
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

void Camera::setToCenterScreen()
{
    auto window = SDL_RenderGetWindow(renderer_);
    int w = 0, h = 0;
    SDL_GetWindowSize(window, &w, &h);
    centerX_ = (float)w / 2.0;
    centerY_ = (float)h / 2.0;
}
