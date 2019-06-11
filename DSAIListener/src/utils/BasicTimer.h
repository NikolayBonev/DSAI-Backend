#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <thread>
#include <atomic>

typedef int TimerID;

typedef std::function<void(TimerID)> TimerCallback;

class BasicTimer
{
private:
    // Timer ID - has to be unique
    TimerID m_nID;

    // Period at which to call TimerCallback
    int m_nPeriod;

    // If the timer is active
    std::atomic_bool m_bRunning;

    // Main Timer Thread, responsible for executing TimerCallback
    std::thread m_thread;

public:

    BasicTimer();
    ~BasicTimer();

    void StartTimer(TimerID nTimerID, int nPeriod, TimerCallback timerCallback);

    void StopTimer(TimerID nTimerID);

    const std::atomic_bool& IsRunning() const;
};

#endif // TIMER_H
