#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <chipmunk/chipmunk.h>
#include <string>


SDL_Renderer* createPreferedRender(SDL_Window* window);


int __stdcall wWinMain(void* hInstance, void* hPrevInstance, wchar_t* lpCmdLine, int nCmdShow)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2"); 
    SDL_Window*   window   = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = createPreferedRender(window);
    
    cpSpace* space = cpSpaceNew();
    cpVect gravity = cpv(0, 100);
    cpSpaceSetGravity(space, gravity);

    cpShape* ground = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(-400, 500), cpv(400, 500), 1);
    cpShapeSetFriction(ground, 1.0);
    cpSpaceAddShape(space, ground);

    cpFloat width   = 30.0;
    cpFloat mass     = 1.0;
    cpFloat moment   = cpMomentForBox(mass, width, width);
    cpBody* boxBody  = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPosition(boxBody, cpv(300, 0));
    cpBodySetAngle(boxBody, 0);
    cpBodySetAngularVelocity(boxBody, 1);
    cpShape* boxShape = cpBoxShapeNew(boxBody, width, width, 0);
    cpShapeSetFriction(boxShape, 0.7);
    //cpShapeSetElasticity(boxShape, 0.8);
    cpSpaceAddShape(space, boxShape);

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        cpSpaceStep(space, 1.0/60.0);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        auto  cp_pos = cpBodyGetPosition(boxBody);
        float centerX = cp_pos.x;
        float centerY = cp_pos.y;
        float size  = width;
        float angle = cpBodyGetAngle(boxBody);

        Sint16 vx[4], vy[4];
        for (int i = 0; i < 4; ++i) {
            vx[i] = centerX + size * cosf(angle + i * M_PI / 2);
            vy[i] = centerY + size * sinf(angle + i * M_PI / 2);
        }
        aapolygonRGBA(renderer, vx, vy, 4, 200, 200, 200, 255);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }


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