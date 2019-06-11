#include "DSAIMgr.h"
#include <csignal>

//Singleton Instance
DSAIMgr* DSAIMgr::m_staticInstance = nullptr;


DSAIMgr* DSAIMgr::GetInstance()
{
    if(!m_staticInstance)
    {
        m_staticInstance = new DSAIMgr();
    }
    return DSAIMgr::m_staticInstance;
}


void DSAIMgr::DestroyInstance()
{
    if(m_staticInstance)
    {
        delete(m_staticInstance);
        m_staticInstance = nullptr;
        std::cout << "Destroying DSAIMgr" << std::endl;
    }
}

DSAIMgr::DSAIMgr():
    m_bRunning(false),
	m_receiver("/dev/ttyUSB0", 9600),
	m_dataProcessor(),
	m_jsonParser(),
    m_server("127.0.0.1", 54000, [this](DSAIListener* listener, int nSocketID, const std::string& strMsg)
{
    if(strMsg == "notification")
    {
        listener->Send(nSocketID, m_jsonParser.GetJson());
    }
})

{

}

DSAIMgr::~DSAIMgr()
{
    Cleanup();
}

bool DSAIMgr::Init()
{    
    //     Constant	                  Explanation
    signal(SIGTERM, SignalHandler);	//termination request, sent to the program
    signal(SIGSEGV, SignalHandler);	//invalid memory access (segmentation fault)
    signal(SIGINT, SignalHandler);	//external interrupt, usually initiated by the user
    signal(SIGILL, SignalHandler);	//invalid program image, such as invalid instruction
    signal(SIGABRT, SignalHandler);	//abnormal termination condition, as is e.g. initiated by std::abort()
    signal(SIGFPE, SignalHandler);	//erroneous arithmetic operation such as divide by zero

    
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
    m_bRunning = false;
    m_receiver.Cleanup();
    m_server.Cleanup();
}

void DSAIMgr::SignalHandler(int nSignal)
{
    std::string strSignal = "";
    switch(nSignal)
    {
        case SIGTERM: strSignal = "SIGTERM"; break;
        case SIGSEGV: strSignal = "SIGSEGV"; break;
        case SIGINT: strSignal = "SIGINT"; break;
        case SIGILL: strSignal = "SIGILL"; break;
        case SIGABRT: strSignal = "SIGABRT"; break;
        case SIGFPE: strSignal = "SIGFPE"; break;
        default: strSignal = "Unknown"; break;
    }
    std::cout << "SIGNAL #" << nSignal << " -> "
              << strSignal << " received!\n";
    DSAIMgr::DestroyInstance();
    exit(nSignal);
}
