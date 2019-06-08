#ifndef BASICJSONPARSER_H_
#define BASICJSONPARSER_H_
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

enum EDataFieldIDs
{
    eSpeed,
    eRPM,
    eEngineTemp,
    eEngineWarning,
    eFogLamp,
    eHazardLamp,
    eGPSLatitude,
    eGPSLongitude,
    eAirTemp,
    eAirHumidity,
    eHandbrake,
    eFuel
};
//temp data structure
struct SDSAIData
{
    int nSpeed = 0;
    int nRPM = 0;
    float fEngineTemp = 0.0f;
    std::string strEngineWarning = "";
    bool bFogLamp = false;
    bool bHazardLamp = false;
    float fGPSLatitude = 0.0f;
    float fGPSLongitude = 0.0f;
    float fAirTemp = 0.0f;
    float fAirHumidity = 0.0f;
    bool bHandbrake = false;
    int nFuel = 0;
};

class BasicJsonParser
{
public:
    BasicJsonParser();
    ~BasicJsonParser();
    // Transform Data structure into JSON string
    void Stringify(const SDSAIData& sData);

    const std::string& GetJson() const;
private:
    std::vector<std::string> m_vecDataIDs;
    std::map<std::string, std::string> m_mapJsonPairs;
    std::string m_strJson;

    template <typename T> void SetValue(EDataFieldIDs eID, const T& value);
    void SetValue(EDataFieldIDs eID, const std::string& value);

    //Set the json pairs from the data structure
    void SetJsonPairs(const SDSAIData& sData);
};

#endif
