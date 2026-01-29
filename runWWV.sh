#!/bin/bash

cd ~/LinuxWWV #edit this if you did not clone the project in your home directory
sudo sh updateRTCtoNTP.sh

g++ -std=c+=17 linuxwwv.cpp -o LinuxWWV
./LinuxWWV
#sudo shutdown --no-wall now
