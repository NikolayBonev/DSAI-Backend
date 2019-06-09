#ifndef DSAIMGR_H_
#define DSAIMGR_H_
#include "utils/BasicJsonParser.h"
#include "DSAISerialListener.h"
#include "DSAIListener.h"

class DSAIMgr
{
public:
    DSAIMgr();
    ~DSAIMgr();

    //Initialize application
    bool Init();

    //Main processing loop
    void Run();

    //Free Resources
    void Cleanup();

private:

    //Used to keep track if the application is running
    bool m_bRunning;

    // Receive automobile data
    DSAISerialListener m_receiver;

    // Transform data into structure
    InfoProcessing m_dataProcessor;

    // Parse structure to json
    BasicJsonParser m_jsonParser;

    //Send Json to clients
    DSAIListener m_server;

};

#endif
