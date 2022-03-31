<!--
 * @Author: mengchaobbbigrui
 * @Date: 2022-03-12 02:28:39
 * @LastEditTime: 2022-03-22 00:59:14
 * @FilePath: /SecureCloudDisk/README.md
-->
SecureCloudDisk

## Install Dependence
①install protobuf
```c
brew install autoconf automake libtool curl make unzip
git clone https://github.com/protocolbuffers/protobuf.git
cd protobuf
git submodule update --init --recursive
./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig
```

②install libevent
```c
brew install libevent
```

③install qt

```c
brew install qt
```
qtcreator:install online

https://download.qt.io/official_releases/online_installers/


