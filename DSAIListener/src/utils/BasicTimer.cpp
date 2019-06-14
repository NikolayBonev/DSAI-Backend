#include "BasicTimer.h"
#include <iostream>

BasicTimer::BasicTimer():
    m_nID(0),
    m_nPeriod(0),
    m_bRunning(false),
    m_thread()
{

}

BasicTimer::~BasicTimer()
{
    if(IsRunning())
    {
        StopTimer(m_nID);
    }
}

void BasicTimer::StartTimer(TimerID nTimerID, int nPeriod, TimerCallback timerCallback)
{
    if(m_nID == nTimerID)
    {
        std::cerr << "Timer " << nTimerID << " already started!\n";
        return;
    }

    m_nID = nTimerID;
    m_nPeriod = nPeriod;
    std::cout << "Timer::Starting Timer #" << m_nID << std::endl;
    m_bRunning = true;
    m_thread = std::thread([this, nTimerID, nPeriod, timerCallback]()
    {
        std::cout << "Entering Timer#" << nTimerID << " on thread #"
                  << m_thread.get_id() << std::endl;
        while (IsRunning())
        {
            timerCallback(nTimerID);
            std::this_thread::sleep_for( std::chrono::milliseconds( nPeriod ) );
        }
        std::cout << "Exiting Timer#" << nTimerID << " on thread #"
                  << m_thread.get_id() << std::endl;
    });
}

void BasicTimer::StopTimer(TimerID nTimerID)
{
    if(!IsRunning())
    {
        std::cout << "Timer #" << m_nID << " is not active for it to be stopped!\n";
        return;
    }
    if(m_nID == nTimerID)
    {
        if(std::this_thread::get_id() == m_thread.get_id())
        {
            std::cout << "StopTimer #" << m_nID <<  " on thread "
            << std::this_thread::get_id() << " THIS IS THE SAME THREAD :O\n";
        }
        try
        {
            m_bRunning = false;
            std::cout << "BasicTimer::Stopping Timer #" << nTimerID << " and detaching thread " << m_thread.get_id() << std::endl;

            m_thread.detach();
        } catch(std::exception& e)
        {
            std::cout << "Exception trying to detach timer #" << m_nID << " on thread " << m_thread.get_id() << ":\n";
            std::cout << e.what() << std::endl;
        }
    }
}

const std::atomic_bool& BasicTimer::IsRunning() const
{
    return m_bRunning;
}
