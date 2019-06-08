#include <iostream>
#include "DSAIListener.h"
#include "utils/BasicJsonParser.h"
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    BasicJsonParser parser;
    SDSAIData sData;
    sData.nSpeed = 1200;
    sData.nRPM = 2333;
    sData.fEngineTemp = 13.6f;
    sData.bFogLamp = true;
    sData.fGPSLatitude = 75.1244f;
    sData.fAirTemp = 41.02f;
    sData.bHandbrake = true;
    sData.nFuel = 1700;
    sData.strEngineWarning = "warning:)";
    parser.Stringify(sData);


    DSAIListener test("192.168.1.6", 54000, [&](DSAIListener* listener, int nSocketID, const std::string& strMsg)
    {
        std::cout << "Echoing message " << parser.GetJson()
                  << " back to Client #" << nSocketID << "\n";
        listener->Send(nSocketID, parser.GetJson());
    });

    if(test.Init())
    {
        test.Run();
    }
    return 0;
}
