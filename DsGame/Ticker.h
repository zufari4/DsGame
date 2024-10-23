#pragma once

#include "TickerQuota.h"
#include <functional>
#include <chrono>
#include <thread>


class Ticker
{
public:
    using Callback = std::function<void()>;

    Ticker();
    ~Ticker();

    void run(Callback cb, std::chrono::nanoseconds frequency);
    void stop();
    void reset();

private:
    void worker(Callback cb);
    void ticker(std::chrono::nanoseconds frequency);

    bool isWork_ = false;
    std::thread workerThread_;
    std::thread tickerThread_;
    TickerQuota quota_;
};

