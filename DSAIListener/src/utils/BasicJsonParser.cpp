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
        "\"handbrake\"",
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

void BasicJsonParser::SetJsonPairs(const VehicleInfo& sData)
{
    SetValue<int>(eSpeed, sData.speedometer);
    SetValue<int>(eRPM, sData.rpm);
    SetValue<float>(eEngineTemp, sData.engineTemperature);
    SetValue(eEngineWarning, sData.DTC);
    SetValue<bool>(eFogLamp, sData.fogLampIndicator);
    SetValue<bool>(eHazardLamp, sData.hazardLights);
    SetValue<float>(eGPSLatitude, sData.gpsLatitude);
    SetValue<float>(eGPSLongitude, sData.gpsLongtitude);
    SetValue<float>(eAirTemp, sData.airTemperature);
    SetValue<float>(eAirHumidity, sData.airHumidity);
    SetValue<bool>(eHandbrake, sData.handBrake);
    SetValue<int>(eFuel, sData.fuelLevel);
}

void BasicJsonParser::Stringify(const VehicleInfo &sData)
{
    SetJsonPairs(sData);
    std::stringstream strStream;
    strStream << "{\n";
    int nCurrPair = 0;
    int nLastPair = m_mapJsonPairs.size() - 1;
    for(const auto& currJsonPair: m_mapJsonPairs)
    {
        strStream << currJsonPair.first << ": " << currJsonPair.second;
        if(nCurrPair < nLastPair)
        {
            strStream << ",\n";
        }
        nCurrPair++;
    }
    strStream << "\n}";
    m_strJson = strStream.str();
//    std::cout << "Stringify->Result:\n" << m_strJson << std::endl;
}

const std::string &BasicJsonParser::GetJson() const
{
    return m_strJson;
}

template<typename T>
void BasicJsonParser::SetValue(EDataFieldIDs eID, const T &value)
{
    if(std::is_same<T,bool>::value)
    {
        std::string strValue = value? "true" : "false";
        m_mapJsonPairs[m_vecDataIDs.at(eID)] = strValue;
    }
    else if(std::is_same<T,int>::value || std::is_same<T,float>::value)
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
