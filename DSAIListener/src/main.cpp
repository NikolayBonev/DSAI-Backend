#include <iostream>
#include "DSAIMgr.h"
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
//    BasicJsonParser parser;
//    SDSAIData sData;
//    sData.nSpeed = 1200;
//    sData.nRPM = 2333;
//    sData.fEngineTemp = 13.6f;
//    sData.bFogLamp = true;
//    sData.fGPSLatitude = 75.1244f;
//    sData.fAirTemp = 41.02f;
//    sData.bHandbrake = true;
//    sData.nFuel = 1700;
//    sData.strEngineWarning = "warning:)";
//    parser.Stringify(sData);
    DSAIMgr backEndApp;
    if(backEndApp.Init())
    {
        backEndApp.Run();
    }
    backEndApp.Cleanup();
    return 0;
}
