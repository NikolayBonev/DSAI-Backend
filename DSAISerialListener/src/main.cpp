#include <iostream>
#include "DSAISerialListener.h"
using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    DSAISerialListener test("/dev/ttyUSB0", 9600);

        test.Init();

        while(1) {
                cout<<"Result:"<<test.Read()<<endl;
        }

    return 0;
}