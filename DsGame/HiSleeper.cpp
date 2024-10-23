#include "HiSleeper.h"
#include <winternl.h>
#include <stdexcept>

#if (_WIN32_WINNT < 0x0A00)  // less Windows 10

static NTSTATUS(__stdcall* NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOL, PLARGE_INTEGER)) GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtDelayExecution");
static NTSTATUS(__stdcall* ZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution) = (NTSTATUS(__stdcall*)(ULONG, BOOLEAN, PULONG)) GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwSetTimerResolution");

void SleepShort(std::chrono::nanoseconds delay)
{
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
#endif

HiSleeper::HiSleeper(std::chrono::nanoseconds delay)
{
    if (delay < std::chrono::nanoseconds(10000)) {
        throw std::runtime_error("Wrong delay for sleep");
    }
    delay_ = delay;
#if (_WIN32_WINNT >= 0x0A00)
    timer_ = CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
    if (timer_ == NULL)
        throw std::runtime_error("Can't init timer");
    delay_ns_ = (delay.count() / 100) - 100;
    li_.QuadPart = -delay_ns_;
#endif
}

HiSleeper::~HiSleeper()
{
#if (_WIN32_WINNT >= 0x0A00)
    if (timer_)
        CloseHandle(timer_);
#endif
}

void HiSleeper::sleep()
{
#if (_WIN32_WINNT >= 0x0A00)
    if (!SetWaitableTimerEx(timer_, &li_, 0, NULL, NULL, NULL, 0)) {
        throw std::runtime_error("Can't set timer");
    }
    WaitForSingleObject(timer_, INFINITE);
#else
    SleepShort(delay_);
#endif
}


