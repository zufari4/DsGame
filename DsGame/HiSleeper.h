#pragma once

#include <winternl.h>

class HiSleeper
{
public:
    HiSleeper(std::chrono::nanoseconds delay)
    {
        timer_ = CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
        if (timer_ == NULL)
            throw std::runtime_error("Can't init timer");
        delay_ns_ = (delay.count() / 100) - 100;
        li_.QuadPart = -delay_ns_;
    }
    ~HiSleeper()
    {
        if (timer_)
            CloseHandle(timer_);
    }
    void sleep()
    {
        if (!SetWaitableTimerEx(timer_, &li_, 0, NULL, NULL, NULL, 0)) {
            CloseHandle(timer_);
            throw std::runtime_error("Can't set timer");
        }
        WaitForSingleObject(timer_, INFINITE);
    }

private:
    HANDLE timer_ = nullptr;
    LARGE_INTEGER li_ = { 0 };
    LONGLONG delay_ns_ = { 0 };
};

static NTSTATUS(__stdcall* NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOL, PLARGE_INTEGER)) GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtDelayExecution");
static NTSTATUS(__stdcall* ZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution) = (NTSTATUS(__stdcall*)(ULONG, BOOLEAN, PULONG)) GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwSetTimerResolution");

static void SleepShort(std::chrono::nanoseconds delay) {
    static bool once = true;
    static LARGE_INTEGER interval;

    if (once) {
        ULONG actualResolution;
        ZwSetTimerResolution(1, true, &actualResolution);
        LONGLONG delay_ns_ = (delay.count() / 100) - 100;
        interval.QuadPart = -delay_ns_;
        once = false;
    }

    NtDelayExecution(false, &interval);
}
