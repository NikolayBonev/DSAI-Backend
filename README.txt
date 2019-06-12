How to build and run DSAI Listener Server.
Prerequisites - Linux based OS, cmake
sudo apt-get install cmake

cd ~/DSAI-Backend/DSAIListener
mkdir build
cd build
cmake ..
make -j4
./DSAIListener

Configuration file is ~/DSAI-Backend/DSAIListener/config.csv

The first item specifies the key we want to configure, while the second is the value for that key.
Currently the only valid and required names for the keys are:
TTYDevice, SerialSpeed, SavePath, ServerIP, ServerPort and TimerPeriod
Note: SavePath will create a file in  ~/DSAI-Backend/DSAIListener/build directory.
Note: TimerPeriod is in milliseconds and must be positive. It's used to notify server clients.

