<!--
 * @Author: mengchaobbbigrui
 * @Date: 2022-03-12 02:28:39
 * @LastEditTime: 2022-03-22 00:59:14
 * @FilePath: /SecureCloudDisk/README.md
-->
SecureCloudDisk

## Install Dependence

①install openssh-server

```c
apt-get install openssh-server
```

②install zlib

```c

wget http://zlib.net/zlib-1.2.11.tar.gz

apt-get install perl automake libtool unzip

tar -xvf zlib-1.2.11.tar.gz

cd zlib-1.2.11/

./configure

make

make install

```

③install openssl

```c

wget https://www.openssl.org/source/openssl-1.1.1.tar.gz

tar -xvf openssl-1.1.1.tar.gz

cd openssl-1.1.1

./config

make

make install

```

④install libevent

```c

git clone https://github.com/libevent/libevent.git

unzip libevent-master.zip

cd libevent-master

./autogen.sh

./configure

make

make install

test/regress > log.txt 验证是否安装成功

```


⑤install QT SDK

```c

wget https://download.qt.io/official_releases/qt/5.15/5.15.0/single/qt-everywhere-src-5.15.0.tar.xz

tar -xvf qt-everywhere-src-5.15.0.tar.xz

cd qt-everywhere-src-5.15.0/

./configure

gmake

make install

```

when install QT SDK will take some problems:

```c

ERROR: The OpenGL functionality tests failed!

You might need to modify the include and library search paths by editing QMAKE_INCDIR_OPENGL[_ES2],
QMAKE_LIBDIR_OPENGL[_ES2] and QMAKE_LIBS_OPENGL[_ES2] in the mkspec for your platform.

```

you can do this when you at configure step:

```c

./configure -no-opengl -skip qtlocation

```
