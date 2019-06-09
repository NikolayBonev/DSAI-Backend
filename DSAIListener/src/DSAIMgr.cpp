#include "DSAIMgr.h"

DSAIMgr::DSAIMgr():
    m_bRunning(false),
    m_server("172.16.1.4", 54000, [this](DSAIListener* listener, int nSocketID, const std::string& strMsg)
{
    std::cout << "Echoing message " << m_jsonParser.GetJson()
              << " back to Client #" << nSocketID << "\n";
    listener->Send(nSocketID, m_jsonParser.GetJson());
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
        const auto& data = m_dataProcessor.GetData("\"speed\":120;\"rpm\":6900;\"engine_temp\":92.6;\"gps_latitude\":120.3445;\"gps_longitude\":120.3445;\"air_temp\":30.5;\"air_humidity\":59;\"fuel\":67;");
        m_jsonParser.Stringify(data);
        m_server.Run();
    }
}

void DSAIMgr::Cleanup()
{
    m_server.Cleanup();
}
