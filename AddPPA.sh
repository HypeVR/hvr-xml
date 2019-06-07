#! /bin/bash

sudo sh -c "echo \"deb [arch=amd64 trusted=yes] http://ppa.hypevr.com debian/\" >> /etc/apt/sources.list"
sudo sh -c "echo Package: \* >> /etc/apt/preferences"
sudo sh -c "echo Pin: origin ppa.hypevr.com  >> /etc/apt/preferences"
sudo sh -c "echo Pin-Priority: 1001 >> /etc/apt/preferences"
sudo apt-get update

# for boost
sudo add-apt-repository ppa:mhier/libboost-latest
sudo apt-get update
