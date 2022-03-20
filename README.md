<!--
 * @Author: mengchaobbbigrui
 * @Date: 2022-03-12 02:28:39
 * @LastEditTime: 2022-03-20 17:23:51
 * @FilePath: /SecureCloudDisk/README.md
-->
SecureCloudDisk
安全云盘

安装openssh-server
apt-get install openssh-server

安装openssl

先安装依赖
apt-get install perl automake libtool unzip

tar -xvf zlib-1.2.11.tar.gz
cd zlib-1.2.11/
./configure
make
make install

tar -xvf openssl-1.1.1.tar.gz
cd openssl-1.1.1
./config
make
make install


安装libevent
unzip libevent-master.zip
cd libevent-master
./autogen.sh  #具体报错具体安装对应的东西
./configure
make
make install


安装qt
wget https://download.qt.io/official_releases/qt/5.15/5.15.0/single/qt-everywhere-src-5.15.0.tar.xz

tar -xvf qt-everywhere-src-5.15.0.tar.xz
cd qt-everywhere-src-5.15.0/
./configure
gmake
make install

报错：
ERROR: The OpenGL functionality tests failed!
You might need to modify the include and library search paths by editing QMAKE_INCDIR_OPENGL[_ES2],
QMAKE_LIBDIR_OPENGL[_ES2] and QMAKE_LIBS_OPENGL[_ES2] in the mkspec for your platform.


./configure -no-opengl

gmake: command not found
ln -s /usr/bin/make /usr/bin/gmake