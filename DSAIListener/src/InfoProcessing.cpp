#include <iostream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <string>
#include "InfoProcessing.hpp"

using namespace std;

InfoProcessing::InfoProcessing()
{
     InitDataVector();
};

InfoProcessing::~InfoProcessing()
{

};

void InfoProcessing::InitDataVector()
{
    InfoProcessing::dataMaping.push_back(make_pair("speed",""));
    InfoProcessing::dataMaping.push_back(make_pair("rpm",""));
    InfoProcessing::dataMaping.push_back(make_pair("engine_temp",""));
    InfoProcessing::dataMaping.push_back(make_pair("engine_warning",""));
    InfoProcessing::dataMaping.push_back(make_pair("fog_lamp",""));
    InfoProcessing::dataMaping.push_back(make_pair("hazard_lamp",""));
    InfoProcessing::dataMaping.push_back(make_pair("gps_latitude",""));
    InfoProcessing::dataMaping.push_back(make_pair("gps_longitude",""));
    InfoProcessing::dataMaping.push_back(make_pair("air_temp",""));
    InfoProcessing::dataMaping.push_back(make_pair("air_humidity",""));
    InfoProcessing::dataMaping.push_back(make_pair("handbrake",""));
    InfoProcessing::dataMaping.push_back(make_pair("fuel",""));
};

void InfoProcessing::split(vector<string> &inputVector,string str, char delimiter)
{
    stringstream sStream(str);
    string splittedString;

    while(getline(sStream, splittedString, delimiter))
    {
        inputVector.push_back(splittedString);
    }
}
void InfoProcessing::ParseData()
{
    const char allDataDelimeter = ';';
    const char singleDataDelimeter = ':';
    vector<string> allSplittedData;
    vector<string> singleDataSplitted;
    int vectorCounter = 0;

    split(allSplittedData,InfoProcessing::vehicleData, allDataDelimeter);

    for(vectorCounter = 0; vectorCounter < allSplittedData.size(); vectorCounter++)
    {
        split(singleDataSplitted,allSplittedData[vectorCounter], singleDataDelimeter);
        if(singleDataSplitted.size() > 1)
        {
            for(vector<pair<string,string>>::iterator variablesIterator=dataMaping.begin(); variablesIterator != dataMaping.end(); variablesIterator++)
            {
                singleDataSplitted[0].erase(std::remove(singleDataSplitted[0].begin(), singleDataSplitted[0].end(), '"'), singleDataSplitted[0].end());

                if(variablesIterator->first.find(singleDataSplitted[0]) != std::string::npos)
                {
                    variablesIterator->second = singleDataSplitted[1];
                }
            }
        }
        singleDataSplitted.clear();
    }
};

void InfoProcessing::DataProcessing()
{
    vector<pair<string,string>>::iterator variablesIterator;

    variablesIterator = InfoProcessing::dataMaping.begin();

    InfoProcessing::publishingData.speedometer = atoi(variablesIterator->second.c_str());
    variablesIterator++;

    InfoProcessing::publishingData.rpm = atoi(variablesIterator->second.c_str());
    variablesIterator++;

    InfoProcessing::publishingData.engineTemperature = atof(variablesIterator->second.c_str());
    variablesIterator++;

    InfoProcessing::publishingData.DTC = variablesIterator->second;
    variablesIterator++;

    if(variablesIterator->second == "true" || variablesIterator->second == "1")
    {
        InfoProcessing::publishingData.fogLampIndicator = true;
    }
    else
    {
        InfoProcessing::publishingData.fogLampIndicator = false;
    }
    variablesIterator++;

    if(variablesIterator->second == "true" || variablesIterator->second == "1")
    {
        InfoProcessing::publishingData.hazardLights = true;
    }
    else
    {
        InfoProcessing::publishingData.hazardLights = false;
    }
    variablesIterator++;

    InfoProcessing::publishingData.gpsLatitude = atof(variablesIterator->second.c_str());
    variablesIterator++;

    InfoProcessing::publishingData.gpsLongtitude = atof(variablesIterator->second.c_str());
    variablesIterator++;

    InfoProcessing::publishingData.airTemperature = atof(variablesIterator->second.c_str());
    variablesIterator++;

    InfoProcessing::publishingData.airHumidity = atof(variablesIterator->second.c_str());
    variablesIterator++;

    if(variablesIterator->second == "true" || variablesIterator->second == "1")
    {
        InfoProcessing::publishingData.handBrake = true;
    }
    else
    {
        InfoProcessing::publishingData.handBrake = false;
    }
    variablesIterator++;

    InfoProcessing::publishingData.fuelLevel = atoi(variablesIterator->second.c_str());
    variablesIterator++;
};

VehicleInfo& InfoProcessing::GetData(const std::string& inputVehicleData)
{
    if(inputVehicleData != "")
    {
        InfoProcessing::vehicleData = inputVehicleData;

        ParseData();

        DataProcessing();
    }

    return InfoProcessing::publishingData;
};
