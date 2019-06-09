#ifndef INFOPROCESSING_HPP_INCLUDED
#define INFOPROCESSING_HPP_INCLUDED
#include <vector>
#include <map>

typedef struct
{
    int speedometer = 0;
    int rpm = 0;
    float engineTemperature = 0;
    std::string DTC = "";
    bool fogLampIndicator = false;
    bool hazardLights = false;
    float gpsLatitude = 0;
    float gpsLongtitude = 0;
    float airTemperature = 0;;
    float airHumidity = 0;
    bool handBrake = false;
    int fuelLevel = 0;
} VehicleInfo;

class InfoProcessing
{
public:
    VehicleInfo publishingData;

    InfoProcessing();

    ~InfoProcessing();

    VehicleInfo& GetData(const std::string& inputVehicleData);

private:
    std::string vehicleData;

    std::vector<std::pair<std::string, std::string>> dataMaping;

    void split(std::vector<std::string> &inputVector,std::string str, char delimiter);

    void InitDataVector();

    void ParseData();

    void DataProcessing();
};

#endif // INFOPROCESSING_HPP_INCLUDED
