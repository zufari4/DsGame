#include <SDL2/SDL.h>
#include "Rectangle.h"
#include "Physics.h"
#include "space.h"
#include <string>
#include <vector>
#include <chrono>


#define SCREEN_W  1000
#define SCREEN_H  800

SDL_Renderer* createPreferedRender(SDL_Window* window);


int wWinMain(void* hInstance, void* hPrevInstance, wchar_t* lpCmdLine, int nCmdShow)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"); 
    SDL_Window*   window   = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = createPreferedRender(window);
    
    Physics physics;
    space_init(physics.getSpace(), SCREEN_W, SCREEN_H);
    physics.run();

    std::vector<Rectangle> rectangles;
    SDL_Event event;
    bool quit = false;

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
                    rectangles.emplace_back(physics.getSpace(), renderer, m.x, m.y, 50, 30);
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
            if (event.type == SDL_MOUSEMOTION) {
                space_mouse_move(physics.getSpace(), event.motion.x, event.motion.y);
            }
            physics.handleEvents(event);
        }
        if (quit) {
            break;
        }

        space_update();
        SDL_SetRenderDrawColor(renderer, 10, 10, 30, 255);
        SDL_RenderClear(renderer);
       
        for (const auto& rectangle : rectangles) {
            rectangle.draw();
        }

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
    const std::string preferedDrivers[3] = { "direct3d12", "direct3d11", "direct3d" };
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
