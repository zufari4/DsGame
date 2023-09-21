#pragma once

#include <inttypes.h>


typedef union SDL_Event;
struct SDL_Renderer;


class Camera
{
public:
    Camera(SDL_Renderer* renderer, float scale = 1);
    void setPosition(float x, float y);
    void move(float dx, float dy);
    void handleEvent(const SDL_Event& event);
    float getOffsetX() const;
    float getOffsetY() const;
    void reset();
    float screenToWorldX(float screenX) const;
    float screenToWorldY(float screenY) const;
    float worldToScreenX(float worldX) const;
    float worldToScreenY(float worldY) const;
    void setScale(float value);
    float getScale() const;

private:
    void setScale(float s, float x, float y);
    void setToCenterScreen();

    SDL_Renderer* renderer_;
    float scale_;
    float offsetX_;
    float offsetY_;
    int32_t dragStartX_;
    int32_t dragStartY_;
    bool  dragIsStart_;
    float centerX_;
    float centerY_;
    float initScale_;
};