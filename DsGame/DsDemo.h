#pragma once


#include "Render.h"
#include "Physics.h"
#include "Camera.h"
#include "FpsTester.h"
#include "DynamicText.h"
#include "StaticText.h"
#include "Rectangle.h"
#include "Grid.h"
#include "MouseBody.h"


class DsDemo
{
public:
    DsDemo();
    bool init();
    void run();
    void free();
    bool isRunning() const;
    void drawLoop();

private:
    void checkEvents();

    bool quit_;
    Render render_;
    Physics physics_;
    std::unique_ptr<Camera> camera_;
    FpsTester fpsTester_;
    std::unique_ptr<DynamicText> fpsText_;
    std::unique_ptr<DynamicText> scaleText_;
    std::unique_ptr<StaticText> helpText1_;
    std::unique_ptr<StaticText> helpText2_;
    std::unique_ptr<StaticText> helpText3_;
    std::unique_ptr<StaticText> helpText4_;
    std::unique_ptr<StaticText> helpText5_;
    std::unique_ptr<StaticText> helpText6_;
    std::vector<std::unique_ptr<Rectangle>> rectagles_;
    std::unique_ptr<Grid> grid_;
    MouseBody mouseBody_;
};

