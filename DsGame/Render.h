#pragma once

#include "IVertexBuffer.h"
#include "Color.h"
#include "Point.h"
#include <inttypes.h>
#include <memory>
#include <vector>


class Camera;
struct SDL_Renderer;
struct SDL_Window;
struct cpSplittingPlane;


class Render 
{
public:
    Render() = delete;
    Render(Render&&) = delete;

    Render(uint8_t clearColorR, uint8_t clearColorG, uint8_t clearColorB);
    bool init();
    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void setDrawColor(const Color& color);
    void drawLine(float x1, float y1, float x2, float y2);
    void drawLineNoScale(const Point2& points);
    void drawLinesNoScale(const Point2* points, int count);
    void drawPoint(float x, float y);
    void drawRectangleFilled(const cpSplittingPlane* planes, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void drawCircleFilled(float centerX, float centerY, const std::vector<float>& xShape, const std::vector<float>& yShape, 
        uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    SDL_Renderer* getRenderer();
    void clear();
    void present();
    bool isSupportMultiThreding() const;
    void getWindowSize(int& w, int& h) const;
    float getScale();
    float getCameraOffsetX();
    float getCameraOffsetY();
    void setCamera(const Camera& camera);
    void free();
private:

    SDL_Renderer* renderer_;
    SDL_Window* window_;
    const Camera* camera_;
    uint8_t clearColorR_;
    uint8_t clearColorG_;
    uint8_t clearColorB_;
    std::unique_ptr<IVertexBuffer> vertexBuffer_;
    bool isSupportMultiThreding_;
};
