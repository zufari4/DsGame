#pragma once

class  Render;
struct cpSpace;
class Camera;

namespace DsMap
{
    void drawDS(Render& render, cpSpace* space, Camera* camera, float x, float y, float size, float padding);
    void drawSubscribe(Render& render, cpSpace* space, Camera* camera, float x, float y, float size, float padding);
    void drawBug(Render& render, cpSpace* space, Camera* camera, float x, float y, float size, float padding);
    void resetPos();
}
