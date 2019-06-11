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
    Cleanup();
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
        while (IsRunning())
        {
            timerCallback(nTimerID);
            std::this_thread::sleep_for( std::chrono::milliseconds( nPeriod ) );
        }
    });
}

void BasicTimer::StopTimer(TimerID nTimerID)
{
    if(m_nID == nTimerID)
    {
        if(std::this_thread::get_id() == m_thread.get_id())
        {
            std::cout << "\t\t\tTHIS IS THE SAME THREAD :O\n\n\n\n";
        }
        try
        {
            std::cout << "Timer::StoppingTimer " << nTimerID << std::endl;
            m_bRunning = false;
            std::cout << "Joining Timer " << m_nID << " thread " << m_thread.get_id() << std::endl;

            //m_thread.join();
            m_thread.detach();
        } catch(std::exception& e)
        {
            std::cout << "Oppls..\n\n\n";
            std::cout << e.what() << std::endl;
        }

        std::cout << "Exiting Timer::StopTimer( " << nTimerID << ")\n\n\n\n";
    }
}

bool BasicTimer::IsRunning() const
{
    return m_bRunning;
}

void BasicTimer::Cleanup()
{
    std::cout << "Timer::Destroying Timer " << m_nID << " thread " << m_thread.get_id() << std::endl;
    m_thread.join(); 
}

