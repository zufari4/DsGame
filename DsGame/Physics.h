#pragma once

#include <atomic>
#include <thread>
#include <inttypes.h>


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
private:
    std::atomic_bool isWork_;
    std::atomic_bool isPause_;
    std::atomic_bool inStep_;
    double gravity_;
    double phyStep_;
    double phyStep_s_;
    cpSpace* space_;
    std::thread mainThread_;
    uint32_t now_;
    uint32_t last_;
    double   accum_;
    bool     needUpdate_;
};
