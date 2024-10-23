#include "TickerQuota.h"

TickerQuota::TickerQuota()
{

}

TickerQuota::~TickerQuota()
{

}

void TickerQuota::add()
{
    std::lock_guard<std::mutex> lock(m);
    q++;
    c.notify_one();
}

int TickerQuota::get()
{
    std::unique_lock<std::mutex> lock(m);
    while (q == 0)
    {
        // release lock as long as the wait and require it afterwards.
        c.wait(lock);
    }
    int val = q;
    q = 0;
    return val;
}