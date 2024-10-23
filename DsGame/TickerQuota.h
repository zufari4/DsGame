#pragma once

#include <mutex>
#include <condition_variable>

class TickerQuota
{
public:
    TickerQuota();
    ~TickerQuota();

    // Add quota
    void add();

    // Get the quota.
    // If the quota is empty, wait till a element is available.
    int get();

private:
    int q = 0;
    std::mutex m;
    std::condition_variable c;
};