#include "BasicJsonParser.h"
BasicJsonParser::BasicJsonParser():
    m_strJson()
{
    m_vecDataIDs =
    {
        "\"speed\"",
        "\"rpm\"",
        "\"engine_temp\"",
        "\"engine_warning\"",
        "\"fog_lamp\"",
        "\"hazard_lamp\"",
        "\"gps_latitude\"",
        "\"gps_longitude\"",
        "\"air_temp\"",
        "\"air_humidity\"",
        "\"handbrake",
        "\"fuel\""
    };

    for(auto strID: m_vecDataIDs)
    {
        m_mapJsonPairs.insert(std::make_pair(strID, ""));
    }
}

BasicJsonParser::~BasicJsonParser()
{
    m_vecDataIDs.erase(m_vecDataIDs.begin(), m_vecDataIDs.end());
    m_mapJsonPairs.erase(m_mapJsonPairs.begin(), m_mapJsonPairs.end());
}

void BasicJsonParser::SetJsonPairs(const SDSAIData &sData)
{
    SetValue<int>(eSpeed, sData.nSpeed);
    SetValue<float>(eEngineTemp, sData.fEngineTemp);
    SetValue(eEngineWarning, sData.strEngineWarning);
    SetValue<bool>(eFogLamp, sData.bFogLamp);
    SetValue<bool>(eHazardLamp, sData.bHazardLamp);
    SetValue<float>(eGPSLatitude, sData.fGPSLatitude);
    SetValue<float>(eGPSLongitude, sData.fGPSLongitude);
    SetValue<float>(eAirTemp, sData.fAirTemp);
    SetValue<float>(eAirHumidity, sData.fAirHumidity);
    SetValue<bool>(eHandbrake, sData.bHandbrake);
    SetValue<int>(eFuel, sData.nFuel);
}

void BasicJsonParser::Stringify(const SDSAIData &sData)
{
    SetJsonPairs(sData);
    std::stringstream strStream;
    strStream << "{\n";
    int nCurrPair = 0;
    int nLastPair = m_mapJsonPairs.size() - 1;
    for(const auto& currJsonPair: m_mapJsonPairs)
    {
        strStream << "" << currJsonPair.first << ":" << currJsonPair.second;
        if(nCurrPair < nLastPair)
        {
            strStream << ",\n";
        }
        nCurrPair++;
    }
    strStream << "\n}";
    m_strJson = strStream.str();
}

const std::string &BasicJsonParser::GetJson() const
{
    return m_strJson;
}

template<typename T>
void BasicJsonParser::SetValue(EDataFieldIDs eID, const T &value)
{
    std::cout << "Setting value of type " << typeid(value).name() << std::endl;

    if(std::is_same<T,bool>::value)
    {
        std::string strValue = value? "true" : "false";
        m_mapJsonPairs[m_vecDataIDs.at(eID)] = strValue;
    }
    else if(std::is_same<T,int>::value)
    {
        m_mapJsonPairs[m_vecDataIDs.at(eID)] = std::to_string(value);
    }
    else if(std::is_same<T,float>::value)
    {
        m_mapJsonPairs[m_vecDataIDs.at(eID)] = std::to_string(value);
    }
    else
    {
        std::cerr << "BasicJsonParser::SetValue Invalide Type "
                  << typeid(value).name() << std::endl;
    }
}

void BasicJsonParser::SetValue(EDataFieldIDs eID, const std::string &value)
{
    std::stringstream strStream;
    strStream << "\"" << value << "\"";
    m_mapJsonPairs[m_vecDataIDs.at(eID)] = strStream.str();
}
