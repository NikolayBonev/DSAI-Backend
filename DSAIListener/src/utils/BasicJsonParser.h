#ifndef BASICJSONPARSER_H_
#define BASICJSONPARSER_H_
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include "../InfoProcessing.hpp"

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

class BasicJsonParser
{
public:
    BasicJsonParser();
    ~BasicJsonParser();
    // Transform Data structure into JSON string
    void Stringify(const VehicleInfo& sData);

    const std::string& GetJson() const;
private:
    std::vector<std::string> m_vecDataIDs;
    std::map<std::string, std::string> m_mapJsonPairs;
    std::string m_strJson;

    template <typename T> void SetValue(EDataFieldIDs eID, const T& value);
    void SetValue(EDataFieldIDs eID, const std::string& value);

    //Set the json pairs from the data structure
    void SetJsonPairs(const VehicleInfo& sData);
};

#endif
