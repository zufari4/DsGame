#include "Ticker.h"

Ticker::Ticker()
{

}

Ticker::~Ticker()
{

}

void Ticker::run(Callback cb, std::chrono::nanoseconds frequency)
{
    isWork_ = true;
    workerThread_ = std::thread(&Ticker::worker, this, cb);
    tickerThread_ = std::thread(&Ticker::ticker, this, frequency);
}

void Ticker::stop()
{
    isWork_ = false;
    workerThread_.join();
    tickerThread_.join();
}

void Ticker::reset()
{
    if (isWork_) {
        int q = quota_.get();
    }
}

void Ticker::worker(Callback cb)
{
    while (isWork_) {
        int q = quota_.get();
        while (isWork_ && q > 0) {
            cb();
            q--;
        }
    }
}

void Ticker::ticker(std::chrono::nanoseconds frequency)
{
    while (isWork_) {
        std::this_thread::sleep_for(frequency);
        quota_.add();
    }
}




