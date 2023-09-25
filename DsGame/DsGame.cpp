#include <SDL2/SDL.h>
#include "Rectangle.h"
#include "Circle.h"
#include "Physics.h"
#include "Walls.h"
#include "DsMap.h"
#include "Camera.h"
#include "Render.h"
#include "StaticText.h"
#include "DynamicText.h"
#include "FpsTester.h"
#include "MouseBody.h"
#include "Utils.h"
#include "Grid.h"
#include <string>
#include <vector>
#include <chrono>


struct Params
{
    Render* render;
    Physics* physics;
    Camera* camera;
    FpsTester* fpsTester;
    DynamicText* fpsText;
    DynamicText* cameraScaleText;
    StaticText* helpText1; 
    StaticText* helpText2;
    StaticText* helpText3;
    StaticText* helpText4;
    StaticText* helpText5;
    StaticText* helpText6;
    bool* quit;
    std::vector<Rectangle>* rectagles;
    Grid* grid;
};


void drawThread(Params& params);
void drawLoop(Params& params);


int wWinMain(void* hInstance, void* hPrevInstance, wchar_t* lpCmdLine, int nCmdShow)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"); 

    int w, h;
    getScreenSize(w, h);
    SDL_Window*   window   = SDL_CreateWindow("DS Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Renderer* renderer = createPreferedRender(window, { "direct3d11", "opengl" });

    Camera camera(renderer, 18);
    Render render(renderer, camera, 31, 31, 31);
    DynamicText fpsText(renderer, 10, 10, 16);
    DynamicText cameraScaleText(renderer, 10, 30, 16);
    StaticText helpText1(renderer, "space        - start/pause", 16, 10, 50 );
    StaticText helpText2(renderer, "r            - restart"    , 16, 10, 70 );
    StaticText helpText3(renderer, "mouse left   - drag object", 16, 10, 90 );
    StaticText helpText4(renderer, "mouse middle - move camera", 16, 10, 110);
    StaticText helpText5(renderer, "mouse wheel  - zoom"       , 16, 10, 130);
    StaticText helpText6(renderer, "mouse right  - add object" , 16, 10, 150);
    Grid grid(render, 250, {50, 50, 60});

    FpsTester fpsTester;
    Physics physics;
    MouseBody mouseBody(physics, camera);
    std::vector<Rectangle> rectangles;

    int winSizeX, winSizeY;
    SDL_GetWindowSize(window, &winSizeX, &winSizeY);
    createWalls(physics, camera.screenToWorldX(winSizeX), camera.screenToWorldY(winSizeY), 1);

    bool quit = false;
    Params params = {
          &render   
        , &physics  
        , &camera   
        , &fpsTester
        , &fpsText  
        , &cameraScaleText
        , &helpText1
        , &helpText2
        , &helpText3
        , &helpText4
        , &helpText5
        , &helpText6
        , &quit 
        , &rectangles
        , &grid
    };
    std::unique_ptr<std::thread> drawThreadPtr;
    if (render.isSupportMultiThreding()) {
        drawThreadPtr = std::make_unique<std::thread>(drawThread, std::ref(params));
    }
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                const SDL_MouseButtonEvent& m = (const SDL_MouseButtonEvent&)(event);
                if (m.button == SDL_BUTTON_RIGHT) {
                    bool prevRun = !physics.isPaused();
                    physics.pause();
                    rectangles.emplace_back(
                        physics.getSpace(),
                        render,
                        camera.screenToWorldX(m.x),
                        camera.screenToWorldY(m.y),
                        5, 5
                    );
                    auto& newRect = rectangles.back();
                    newRect.setMass(3);
                    newRect.setColor(rnd(0, 255), rnd(0, 255), rnd(0, 255), 255);
                    if (prevRun) physics.resume();
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                const SDL_KeyboardEvent& ev = (const SDL_KeyboardEvent&)(event);
                if (ev.keysym.scancode == SDL_SCANCODE_SPACE) {
                    if (physics.isWork()) {
                        if (physics.isPaused()) physics.resume();
                        else physics.pause();
                    }
                    else {
                        physics.run();
                    }
                }
                else if (ev.keysym.scancode == SDL_SCANCODE_R) {
                    physics.pause();
                    camera.reset();
                    DsMap::resetPos();
                    physics.invalidate();
                }
                else if (ev.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                    break;
                }
            }
            physics.handleEvents(event);
            camera.handleEvent(event);
            mouseBody.handleEvents(event);
            grid.handleEvents(event);
        }
        if (!render.isSupportMultiThreding()) {
            drawLoop(params);
        }
    }

    if (drawThreadPtr) drawThreadPtr->join();
    physics.stop();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void drawLoop(Params& params)
{
    params.render->clear();

    params.grid->draw();

    DsMap::drawDS(*params.render, params.physics->getSpace(), params.camera, 35, 5, 0.8, 0.2);
    DsMap::drawBug(*params.render, params.physics->getSpace(), params.camera, 40, 50, 0.3, 0.1);
    DsMap::drawSubscribe(*params.render, params.physics->getSpace(), params.camera, 22, -200, 0.3, 0.1);
    for (auto& rectangle : *params.rectagles) {
        rectangle.draw();
    }

    params.fpsText->draw("FPS: %u", params.fpsTester->getFps());
    params.cameraScaleText->draw("scale: %.0f", params.camera->getScale());
    params.helpText1->draw();
    params.helpText2->draw();
    params.helpText3->draw();
    params.helpText4->draw();
    params.helpText5->draw();
    params.helpText6->draw();

    params.render->present();
    params.fpsTester->loop();
}

void drawThread(Params& params)
{
    while (!(*params.quit)) {
        drawLoop(params);
    }
}
