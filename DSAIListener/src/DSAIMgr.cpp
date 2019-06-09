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
        std::cerr << "DSAI Server failed to init" << std::endl;
        return false;
    }

    if(!m_receiver.Init())
    {
        std::cerr << "DSAISerialListener failed to initialize" << std::endl;
        return false;
    }

    m_bRunning = true;
    return true;
}

void DSAIMgr::Run()
{
    while (m_bRunning)
    {
        std::string strAutoData = m_receiver.Read();
        const auto& data = m_dataProcessor.GetData(strAutoData);
        m_jsonParser.Stringify(data);
        m_server.Run();
    }
}

void DSAIMgr::Cleanup()
{
    m_receiver.Cleanup();
    m_server.Cleanup();
}
