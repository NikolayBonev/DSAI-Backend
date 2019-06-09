#ifndef DSAIMGR_H_
#define DSAIMGR_H_
#include "DSAIListener.h"
#include "utils/BasicJsonParser.h"
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
    DSAIListener m_server;
    BasicJsonParser m_jsonParser;

};

#endif
