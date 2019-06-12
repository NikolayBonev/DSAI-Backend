#include "BasicConfig.h"
#include <iostream>

/* Configuration Keys */
#define TTY_DEVICE "TTYDevice"
#define SERIAL_SPEED "SerialSpeed"
#define SAVE_PATH "SavePath"
#define SERVER_IP "ServerIP"
#define SERVER_PORT "ServerPort"
#define TIMER_PERIOD "TimerPeriod"

BasicConfig::BasicConfig()
{
}

BasicConfig::~BasicConfig()
{
}

bool BasicConfig::Init(const std::string& strFilePath)
{
	std::ifstream iStream;
	iStream.open(strFilePath);
    std::cout << "BasicConfig::Init(" << strFilePath << ")\n";

	if (!iStream.is_open())
	{
		std::cerr << "BasicConfig::Error opening " << strFilePath << "\n";
        iStream.close();
		return false;
	}

	for (CSVIterator it(iStream); it != CSVIterator(); ++it)
	{
		std::string strKey = (*it)[0];
		std::string strValue = (*it)[1];
        std::cout << strKey << ": " << strValue << std::endl;
        
        if(strKey == TTY_DEVICE)
        {
            m_config.strTTYDevice = strValue;
        }
        else if(strKey == SERIAL_SPEED)
        {
            m_config.nSerialSpeed = atoi(strValue.c_str());
        }
        else if(strKey == SAVE_PATH)
        {
            m_config.strSavePath = strValue;
        }
        else if(strKey == SERVER_IP)
        {
            m_config.strServerIP = strValue;
        }
        else if(strKey == SERVER_PORT)
        {
            m_config.nServerPort = atoi(strValue.c_str());
        }
        else if(strKey == TIMER_PERIOD)
        {
            m_config.nTimerPeriod = atoi(strValue.c_str());
        }
        else
        {
            std::cerr << "Unrecognized condiguration key! Terminating program!" << std::endl;
            iStream.close();
            return false;
        }
	}
	
	if(!m_config.IsConfigured())
    {
        std::cerr << "One or more fields are not configured!" << std::endl;
        iStream.close();
        return false;
    }

	iStream.close();
	return true;

}

const SConfig& BasicConfig::GetConfig() const
{
    return m_config;
}
