#include <iostream>
#include "DSAIMgr.h"

int main()
{
    DSAIMgr* ptrDSAIMgr = DSAIMgr::GetInstance();
    if(ptrDSAIMgr->Init())
    {
        ptrDSAIMgr->Run();
    }
    
    DSAIMgr::DestroyInstance();
    ptrDSAIMgr = nullptr;
    
    return 0;
}
