How to build and run DSAI Listener Server.
Prerequisites - Linux based OS, cmake
sudo apt-get install cmake

cd ~/DSAI-Backend/DSAIListener
mkdir build
cd build
cmake ..
make -j4
./DSAIListener
