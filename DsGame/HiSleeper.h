#pragma once

#include "WindowsHeaders.h"
#include <chrono>

class HiSleeper
{
public:
    HiSleeper(std::chrono::nanoseconds delay);
    ~HiSleeper();
    void sleep();

private:
    std::chrono::nanoseconds delay_;
    HANDLE timer_ = nullptr;
    LARGE_INTEGER li_ = { 0 };
    LONGLONG delay_ns_ = { 0 };
};
