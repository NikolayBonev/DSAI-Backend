#include <iostream>
#include "DSAIListener.h"
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    DSAIListener test("127.0.0.1", 54000, [](DSAIListener* listener, int nSocketID, const std::string& strMsg)
    {
        std::cout << "Echoing message " << strMsg
                  << " back to Client #" << nSocketID << "\n";
        listener->Send(nSocketID, strMsg);
    });

    if(test.Init())
    {
        test.Run();
    }
    return 0;
}
