#pragma once

#include <memory>
#include <inttypes.h>


class HiResTimer;

class FpsTester
{
public:
    FpsTester();
    void loop();
    uint32_t getFps() const;
private:
    uint32_t fps_;
    uint32_t counter_;
    std::unique_ptr<HiResTimer> timer_;
    double prevFrameTime_;
    double elapsed_;
    double now_;
};