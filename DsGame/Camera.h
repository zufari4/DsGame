#pragma once

#include <inttypes.h>


typedef union SDL_Event;
struct SDL_Renderer;


class Camera
{
public:
    Camera(SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& event);
    float getOffsetX() const;
    float getOffsetY() const;
    void reset();
    float screenToWorldX(float screenX) const;
    float screenToWorldY(float screenY) const;
private:
    void setScale(float s);
    SDL_Renderer* renderer_;
    float scale_;
    float offsetX_;
    float offsetY_;
    int32_t dragStartX_;
    int32_t dragStartY_;
    bool  dragIsStart_;
};