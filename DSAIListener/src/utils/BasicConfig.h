#ifndef BASIC_CONFIG_H_
#define BASIC_CONFIG_H_
#include "CSVIterator.h"

struct SConfig
{
    std::string strTTYDevice = "";
    int nSerialSpeed = -1;
    std::string strSavePath = "";
    std::string strServerIP = "";
    int nServerPort = -1;
    int nTimerPeriod = -1;
    
    bool IsConfigured()
    {
        bool bConfigured = (strTTYDevice != "") &&
                            (nSerialSpeed != -1) &&
                            (strSavePath != "") &&
                            (strServerIP != "") &&
                            (nServerPort != -1) &&
                            (nTimerPeriod > 0);
        return bConfigured;
    }
};

class BasicConfig
{
public:
    BasicConfig();
    ~BasicConfig();
    
    bool Init(const std::string& strFilePath);
    const SConfig& GetConfig() const;
private:
    SConfig m_config;
};

#endif
