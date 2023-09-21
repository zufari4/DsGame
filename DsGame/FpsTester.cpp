#include "FpsTester.h"
#include "HiResTimer.h"


FpsTester::FpsTester()
    : fps_(0)
    , counter_(0)
    , timer_(std::make_unique<HiResTimer>())
    , prevFrameTime_(timer_->now())
    , elapsed_(0)
{

}

void FpsTester::loop()
{
    ++counter_;
    now_ = timer_->now();
    elapsed_ += now_ - prevFrameTime_;
    prevFrameTime_ = now_;
    if (elapsed_ >= 1.0) {
        fps_ = counter_;
        counter_ = 0;
        elapsed_ = 0;
    }
}

uint32_t FpsTester::getFps() const
{
    return fps_;
}
