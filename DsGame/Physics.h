#pragma once

#include <atomic>
#include <thread>
#include <inttypes.h>
#include "HiResTimer.h"

struct cpSpace;
typedef union SDL_Event;

class Physics
{
public:
    Physics();
    void run();
    void stop();
    void pause();
    void resume();
    void handleEvents(const SDL_Event& event);
    cpSpace* getSpace();
    void step();
    bool isWork() const;
    bool isPaused() const;
    void setStepFrequency(double hz);
    void resetTime();
private:
    std::atomic_bool isWork_;
    std::atomic_bool isPause_;
    std::atomic_bool inStep_;
    double gravity_;
    double stepDuration_;
    cpSpace* space_;
    std::thread mainThread_;
    double now_;
    double last_;
    double accum_;
    double dt_;
    const double maxDT_;
    HiResTimer timer_;
};
