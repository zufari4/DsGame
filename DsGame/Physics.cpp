#include "Physics.h"
#include "HiResTimer.h"
#include <chipmunk/chipmunk_private.h>
#include <chipmunk/chipmunk.h>
#include <chipmunk/cpHastySpace.h>
#include <chrono>


void physicThread(Physics& physics)
{
    physics.resetTime();
    while (physics.isWork())
    {
        physics.step();
    }
}


Physics::Physics()
    : isWork_(false)
    , isPause_(true)
    , inStep_(false)
    , gravity_(100)
    , stepDuration_(0)
    , space_(cpHastySpaceNew())
    , now_(0)
    , last_(0)
    , accum_(0)
    , dt_(0)
    , maxDT_(0.2)
{
    cpSpaceSetGravity(space_, cpv(0, gravity_));
    cpSpaceSetIterations(space_, 30);
    setStepFrequency(60);
}

void Physics::run()
{
    isWork_  = true;
    isPause_ = false;
    mainThread_ = std::make_unique<std::thread>(physicThread, std::ref(*this));
}

void Physics::stop()
{
    isWork_ = false;
    if (mainThread_) mainThread_->join();
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
        std::this_thread::sleep_for(std::chrono::milliseconds(18));
        resetTime();
        return;
    }

    now_  = timer_.now();
    dt_   = now_ - last_;
    last_ = now_;
    if (dt_ > maxDT_) dt_ = maxDT_;

    for (accum_ += dt_; accum_ >= stepDuration_; accum_ -= stepDuration_) {
        invalidate();
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
    bool isPrevPause = isPause_;
    pause();
    stepDuration_ = 1.0/hz;
    if (isPrevPause) resume();
}

void Physics::resetTime() 
{
    now_   = timer_.now();
    last_  = now_;
    dt_    = 0;
    accum_ = 0;
}

void Physics::invalidate()
{
    cpHastySpaceStep(space_, stepDuration_);
}
