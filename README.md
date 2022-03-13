<!--
 * @Author: your name
 * @Date: 2022-03-12 02:28:39
 * @LastEditTime: 2022-03-14 00:44:39
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /SecureCloudDisk/README.md
-->
SecureCloudDisk
安全云盘

安装openssh-server


安装openssl

先安装依赖
yum install perl automake libtool unzip

tar -xvf zlib-1.2.11.tar.gz
cd zlib-1.2.11/
./configure
make
make install

tar -xvf openssl-1.1.1.tar.gz
cd openssl-1.1.1
./config

export LC_CTYPE=en_US.UTF-8
export LC_ALL=en_US.UTF-8

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
chmod 777 qt-opensource-linux-x64-5.12.4.run
./qt-opensource-linux-x64-5.12.4.run

// 报失败，找不到libx11,执行如下两条命令
yum update
yum install libx11-xcb-dev