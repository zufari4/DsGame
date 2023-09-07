#include "Physics.h"
#include <SDL2/SDL.h>
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>
#include <chipmunk/cpHastySpace.h>
#include "space.h"
#include <chrono>


void physicThread(Physics& physics)
{
    while (physics.isWork())
    {
        physics.step();
    }
}


Physics::Physics()
    : isWork_(false)
    , isPause_(true)
    , inStep_(false)
    , gravity_(9.8)
    , phyStep_(0)
    , phyStep_s_(0)
    , space_(cpHastySpaceNew())
    , now_(0)
    , last_(SDL_GetTicks())
    , accum_(0)
    , needUpdate_(false)
{
    //cpSpaceSetIterations(space, 5);
    cpSpaceSetGravity(space_, cpv(0, gravity_));
    setStepFrequency(60);
}

void Physics::run()
{
    isWork_  = true;
    isPause_ = false;
    mainThread_ = std::thread(physicThread, std::ref(*this));
}

void Physics::stop()
{
    isWork_ = false;
    mainThread_.join();
    cpSpaceFree(space_);
}

void Physics::pause()
{
    isPause_ = true;
    while (inStep_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(18));
    }
}

void Physics::resume()
{
    isPause_ = false;
}

void Physics::handleEvents(const SDL_Event& event)
{
}

cpSpace* Physics::getSpace()
{
    return space_;
}

void Physics::step()
{
    inStep_ = true;

    if (isPause_) {
        inStep_ = false;
        accum_ = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(18));
        last_ = SDL_GetTicks();
        return;
    }

    now_    = SDL_GetTicks();
    accum_ += (now_ - last_);
    last_   = now_;
    if (accum_ > 160.0) accum_ = phyStep_;
    needUpdate_ = accum_ >= phyStep_;

    while (accum_ >= phyStep_) {
        cpHastySpaceStep(space_, phyStep_s_);
        accum_ -= phyStep_;
    }

    inStep_ = false;
}

bool Physics::isWork() const
{
    return isWork_;
}

bool Physics::isPaused() const
{
    return isPause_;
}

void Physics::setStepFrequency(double hz)
{
    pause();
    phyStep_   = 1000.0/hz;
    phyStep_s_ = 1.0/hz;
    resume();
}
