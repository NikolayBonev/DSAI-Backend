#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>
#include "InfoProcessing.hpp"

using namespace std;


int main()
{
    string str = "\"speed\":20;\"rpm\":1200;\"engine_temp\":90;\"engine_warning\":engine idling error;\"fog_lamp\":1;\"hazard_lamp\":false;\"gps_latitude\":33;\"gps_longitude\":66;\"air_temp\":25;\"air_humidity\":50;\"handbrake\":1;\"fuel\":53;";
    VehicleInfo vinfo;
    InfoProcessing info;

    vinfo = info.GetData(str);

    cout<<vinfo.speedometer;
    cout<<"\n";

    cout<<vinfo.rpm;
    cout<<"\n";

    cout<<vinfo.engineTemperature;
    cout<<"\n";

    cout<<vinfo.DTC;
    cout<<"\n";

    cout<<vinfo.fogLampIndicator;
    cout<<"\n";

    cout<<vinfo.hazardLights;
    cout<<"\n";

    cout<<vinfo.gpsLatitude;
    cout<<"\n";

    cout<<vinfo.gpsLongtitude;
    cout<<"\n";

    cout<<vinfo.airTemperature;
    cout<<"\n";

    cout<<vinfo.airHumidity;
    cout<<"\n";

    cout<<vinfo.handBrake;
    cout<<"\n";

    cout<<vinfo.fuelLevel;
    cout<<"\n";

    return 0;
}
