#include "DSAIMgr.h"
#include <csignal>

//Singleton Instance
DSAIMgr* DSAIMgr::m_staticInstance = nullptr;
bool DSAIMgr::m_bRunning = false;

//path to configuration
#define CONFIG_PATH "config.csv"

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
		m_staticInstance->m_operator.writeDataToFile("");
        delete(m_staticInstance);
        m_staticInstance = nullptr;
        std::cout << "Destroying DSAIMgr" << std::endl;
    }
}

DSAIMgr::DSAIMgr():
    m_basicConfig(),
	m_receiver(),
	m_dataProcessor(),
	m_operator(),
	m_jsonParser(),
    m_server([this](DSAIListener* listener, int nSocketID, const std::string& strMsg)
    {
        if(strMsg == "notification")
        {
            listener->Send(nSocketID, m_jsonParser.GetJson());
        }
        else
        {
            listener->Send(nSocketID, strMsg);
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

    if(!m_basicConfig.Init(CONFIG_PATH))
    {
        std::cerr << "DSAI Configuration failed to init!" << std::endl;
        return false;
    }

    const SConfig& config = m_basicConfig.GetConfig();

    if(!m_receiver.Init(config.strTTYDevice, config.nSerialSpeed))
    {
        std::cerr << "DSAISerialListener failed to initialize" << std::endl;
        return false;
    }
	
    if (!m_operator.Init(config.strSavePath))
    {
            std::cerr << "DSAIFileOperator failed to initialize" << std::endl;
            return false;
    }

    if(!m_server.Init(config.strServerIP, config.nServerPort, config.nTimerPeriod))
    {
        std::cerr << "DSAI Server failed to init!" << std::endl;
        return false;
    }
	
	recoveryData = m_operator.readDataFromFile();

	if (recoveryData.empty())
	{
		isFailed = false;
	}
	else
	{
        isFailed = true;
	}

    m_bRunning = true;
    return true;
}

void DSAIMgr::Run()
{
    while (m_bRunning)
    {
		if (recoveryData.empty() || (!recoveryData.empty() && !isFailed))
        {
                recoveryData = m_receiver.Read();
                m_operator.writeDataToFile(recoveryData);
        }
		
        const auto& data = m_dataProcessor.GetData(recoveryData);
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
    /* Only handle one signal before terminating */
    if(!m_bRunning)
    {
        return;
    }

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
