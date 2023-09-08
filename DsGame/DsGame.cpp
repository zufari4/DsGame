#include <SDL2/SDL.h>
#include "Rectangle.h"
#include "Circle.h"
#include "Physics.h"
#include "space.h"
#include "DsMap.h"
#include "Camera.h"
#include <string>
#include <vector>
#include <chrono>


SDL_Renderer* createPreferedRender(SDL_Window* window);


int wWinMain(void* hInstance, void* hPrevInstance, wchar_t* lpCmdLine, int nCmdShow)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"); 
    SDL_DisplayMode dm;

    if (SDL_GetDesktopDisplayMode(0, &dm))
    {
        printf("Error getting desktop display mode\n");
        return -1;
    }

    SDL_Window*   window   = SDL_CreateWindow("DS Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = createPreferedRender(window);
    Camera camera(renderer);

    Physics physics;
    int winSizeX, winSizeY;
    SDL_GetWindowSize(window, &winSizeX, &winSizeY);
    space_init(physics.getSpace(), winSizeX, winSizeY);

    SDL_Event event;
    bool quit = false;

    std::vector<Cirle> circles;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
                const SDL_MouseButtonEvent& m = (const SDL_MouseButtonEvent&)(event);
                if (m.button == SDL_BUTTON_RIGHT) {
                    physics.pause();
                    //circles.emplace_back(physics.getSpace(), renderer, m.x, m.y, 20);
                    physics.resume();
                }
                else {
                    physics.pause();
                    space_mouse_up(physics.getSpace());
                    physics.resume();
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                const SDL_MouseButtonEvent& m = (const SDL_MouseButtonEvent&)(event);
                if (m.button == SDL_BUTTON_LEFT) {
                    physics.pause();
                    space_mouse_down(physics.getSpace());
                    physics.resume();
                }
            }
            else if (event.type == SDL_MOUSEMOTION) {
                space_mouse_move(physics.getSpace(), camera.screenToWorldX(event.motion.x), camera.screenToWorldY(event.motion.y));
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
                    DsMap::resetPos();
                    camera.reset();
                    physics.resume();
                }
                else if (ev.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                    break;
                }
            }
            physics.handleEvents(event);
            camera.handleEvent(event);
        }
        if (quit) {
            break;
        }

        SDL_SetRenderDrawColor(renderer, 31, 31, 31, 255);
        SDL_RenderClear(renderer);
       
        DsMap::drawDS(renderer, physics.getSpace(), &camera, 20, 20, 10, 1);

        SDL_RenderPresent(renderer);
    }

    physics.stop();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

SDL_Renderer* createPreferedRender(SDL_Window* window)
{ 
    const std::string preferedDrivers[3] = {"direct3d11", "direct3d", "opengl"};
    SDL_Renderer*     renderer = nullptr;
    SDL_RendererInfo  rendererInfo;

    for (const auto& driver : preferedDrivers) {
        for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i) {
            SDL_GetRenderDriverInfo(i, &rendererInfo);
            if (rendererInfo.name == driver) {
                return SDL_CreateRenderer(window, i, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            }
        }  
    }

    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}
