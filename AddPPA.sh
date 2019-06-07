#! /bin/bash

sudo sh -c "echo \"deb [arch=amd64 trusted=yes] http://ppa.hypevr.com debian/\" >> /etc/apt/sources.list"
sudo sh -c "echo Package: \* >> /etc/apt/preferences"
sudo sh -c "echo Pin: origin ppa.hypevr.com  >> /etc/apt/preferences"
sudo sh -c "echo Pin-Priority: 1001 >> /etc/apt/preferences"
apt-get update