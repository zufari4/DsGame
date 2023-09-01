#pragma once

#include <SDL2/SDL.h>
#include <atomic>
#include <thread>


struct cpSpace;


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
    void setSpeedHz(double hz);
private:
    std::atomic_bool isWork_;
    std::atomic_bool isPause_;
    std::atomic_bool inStep_;
    double speedHz_;
    double gravity_;
    double phyStep_;
    double phyStep_s_;
    cpSpace* space_;
    std::thread mainThread_;
    Uint32 now_;
    Uint32 last_;
    double accum_;
    bool   needUpdate_;
};
