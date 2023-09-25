#include "DsDemo.h"
#include "Walls.h"
#include "DsMap.h"
#include "Utils.h"
#include <SDL2/SDL_events.h>


DsDemo::DsDemo()
    : quit_(false)
    , physics_(100)
    , render_(31, 31, 31)
    , mouseBody_(physics_)
{
}

bool DsDemo::init()
{
    if (!render_.init()) {
        return false;
    }

    int winW, winH;
    render_.getWindowSize(winW, winH);
    
    camera_ = std::make_unique<Camera>(winW, winH, 18);
    render_.setCamera(*camera_);
    mouseBody_.setCamera(*camera_);

    auto renderer = render_.getRenderer();

    fpsText_   = std::make_unique<DynamicText>(renderer, 10, 10, 16);
    scaleText_ = std::make_unique<DynamicText>(renderer, 10, 30, 16);
    helpText1_ = std::make_unique<StaticText>(renderer, "space        - start/pause", 16, 10, 50);
    helpText2_ = std::make_unique<StaticText>(renderer, "r            - restart", 16, 10, 70);
    helpText3_ = std::make_unique<StaticText>(renderer, "mouse left   - drag object", 16, 10, 90);
    helpText4_ = std::make_unique<StaticText>(renderer, "mouse middle - move camera", 16, 10, 110);
    helpText5_ = std::make_unique<StaticText>(renderer, "mouse wheel  - zoom", 16, 10, 130);
    helpText6_ = std::make_unique<StaticText>(renderer, "mouse right  - add object", 16, 10, 150);

    Color gridColor(50, 50, 60);
    grid_      = std::make_unique<Grid>(render_, 250, gridColor);
    Walls::create(physics_, camera_->screenToWorldX(winW), camera_->screenToWorldY(winH), 1);
}

void drawThread(DsDemo& demo)
{
    while (demo.isRunning()) {
        demo.drawLoop();
    }
}


void DsDemo::run()
{
    std::unique_ptr<std::thread> drawThreadPtr;
    if (render_.isSupportMultiThreding()) {
        drawThreadPtr = std::make_unique<std::thread>(drawThread, std::ref(*this));
    }

    while (isRunning()) {
        checkEvents();
        if (!render_.isSupportMultiThreding()) {
            drawLoop();
        }
    }

    if (drawThreadPtr) drawThreadPtr->join();
}

void DsDemo::free()
{
    physics_.stop();
    rectagles_.clear();
    DsMap::free();
    Walls::free();
    physics_.free();
    render_.free();
}

bool DsDemo::isRunning() const
{
    return !quit_;
}

void DsDemo::drawLoop()
{
    render_.clear();

    grid_->draw();

    DsMap::drawDS(render_, physics_.getSpace(), camera_.get(), 35, 5, 0.8, 0.2);
    DsMap::drawBug(render_, physics_.getSpace(), camera_.get(), 40, 50, 0.3, 0.1);
    DsMap::drawSubscribe(render_, physics_.getSpace(), camera_.get(), 22, -200, 0.3, 0.1);
    for (auto& rectangle : rectagles_) {
        rectangle->draw();
    }

    fpsText_->draw("FPS: %u", fpsTester_.getFps());
    scaleText_->draw("scale: %.0f", camera_->getScale());
    helpText1_->draw();
    helpText2_->draw();
    helpText3_->draw();
    helpText4_->draw();
    helpText5_->draw();
    helpText6_->draw();

    render_.present();
    fpsTester_.loop();
}

void DsDemo::checkEvents()
{
    static SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit_ = true;
            break;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            const SDL_MouseButtonEvent& m = (const SDL_MouseButtonEvent&)(event);
            if (m.button == SDL_BUTTON_RIGHT) {
                bool prevRun = !physics_.isPaused();
                physics_.pause();
                auto newRect = std::make_unique<Rectangle>(
                    physics_.getSpace(),
                    render_,
                    camera_->screenToWorldX(m.x),
                    camera_->screenToWorldY(m.y),
                    5, 5
                );
                newRect->setMass(3);
                newRect->setColor(rnd(0, 255), rnd(0, 255), rnd(0, 255), 255);
                rectagles_.push_back(std::move(newRect));
                if (prevRun) physics_.resume();
            }
        }
        else if (event.type == SDL_KEYDOWN) {
            const SDL_KeyboardEvent& ev = (const SDL_KeyboardEvent&)(event);
            if (ev.keysym.scancode == SDL_SCANCODE_SPACE) {
                if (physics_.isWork()) {
                    if (physics_.isPaused()) physics_.resume();
                    else physics_.pause();
                }
                else {
                    physics_.run();
                }
            }
            else if (ev.keysym.scancode == SDL_SCANCODE_R) {
                physics_.pause();
                int winW, winH;
                render_.getWindowSize(winW, winH);
                camera_->reset(winW, winH);
                DsMap::resetPos();
                rectagles_.clear();
                physics_.invalidate();
            }
            else if (ev.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                quit_ = true;
                break;
            }
        }
        physics_.handleEvents(event);
        camera_->handleEvent(event);
        mouseBody_.handleEvents(event);
        grid_->handleEvents(event);
    }
}
