#include "DSAIMgr.h"

DSAIMgr::DSAIMgr():
    m_bRunning(false),
    m_server("172.16.1.4", 54000, [&](DSAIListener* listener, int nSocketID, const std::string& strMsg)
{
    std::cout << "Echoing message " << strMsg
              << " back to Client #" << nSocketID << "\n";
    listener->Send(nSocketID, strMsg);
}),
    m_jsonParser()
{

}

DSAIMgr::~DSAIMgr()
{
    Cleanup();
}

bool DSAIMgr::Init()
{
    if(!m_server.Init())
    {
        return false;
    }

    m_bRunning = true;
    return true;
}

void DSAIMgr::Run()
{
    while (m_bRunning)
    {
        m_server.Run();
    }
}

void DSAIMgr::Cleanup()
{
    m_server.Cleanup();
}
