# python-screen-capture-lite
**NB: Currently only for Linux systems With X11**

## Requirements
* pybind11 >= 2.9.0
* ninja
* cmake >= 2.20.0
* libX11-dev libxinerma-dev libxfixes-dev libxtst-dev
* GCC >= 10
* OpenCV4


## Installation
`pip install git+https://github.com/sysx-labs/python-screen-capture-lite`

## Dependencies Installation
Use with care:
```bash
apt-get update && \
    apt-get install wget default-jre xdotool -y && \
    apt-get install -y software-properties-common lsb-release && \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
    sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" && \
    sudo apt install kitware-archive-keyring && sudo rm /etc/apt/trusted.gpg.d/kitware.gpg && \
    apt-get install python3-pip cmake -y && \
    pip3 install ninja && \
    apt update && \
    apt install libx11-dev libopencv-dev -y && \
    apt install libxinerama-dev libxfixes-dev libxtst-dev  gcc-10 g++-10 cpp-10 -y && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 100 --slave /usr/bin/g++ g++ /usr/bin/g++-10 --slave /usr/bin/gcov gcov /usr/bin/gcov-10 && \
    pip3 install "pybind11[global]" && \
    apt-get clean all
```
