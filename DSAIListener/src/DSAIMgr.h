#ifndef DSAIMGR_H_
#define DSAIMGR_H_
#include "utils/BasicConfig.h"
#include "utils/BasicJsonParser.h"
#include "DSAISerialListener.h"
#include "DSAIListener.h"
#include "DSAIFileOperator.h"

class DSAIMgr
{
public:
    ~DSAIMgr();

    //Initialize application
    bool Init();

    //Main processing loop
    void Run();

    //Free Resources
    void Cleanup();
    
    //Handle linux kernel signals and terminate program
    static void SignalHandler(int nSignal);
    
    //Get a single instance of this class
    static DSAIMgr* GetInstance();
    
    //Destroy the instance
    static void DestroyInstance();

	// Keeps current read data.
	std::string recoveryData;
	
	// Flag is the process started after fail.
	bool isFailed;
	
private:

    //Used to keep track if the application is running
    static bool m_bRunning;

    // Configure application
    BasicConfig m_basicConfig;

    // Receive automobile data
    DSAISerialListener m_receiver;

    // Transform data into structure
    InfoProcessing m_dataProcessor;

    // Parse structure to json
    BasicJsonParser m_jsonParser;

    //Send Json to clients
    DSAIListener m_server;
	
	// Backup of the last read data.
	DSAIFileOperator m_operator;
    
    //Private constructor for Singleton
    DSAIMgr();
    
    //Singleton Instance
    static DSAIMgr* m_staticInstance;

};

#endif
