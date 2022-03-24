/*
 * @Author: mengchaobbbigrui
 * @Date: 2022-03-20 19:45:38
 * @LastEditTime: 2022-03-20 23:12:54
 * @LastEditors: Please set LastEditors
 * @Description: TcpSClient.cpp
 * @FilePath: /code/SecureCloudDisk/TCP/TCPServer/TcpSClient.cpp
 */
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv)
{
    unsigned short port = 8080;
    const char *ip = "127.0.0.1";
    if (argc > 2) {
        ip = argv[1];
        port = atoi(argv[2]);
    }

    // 1.create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock <= 0) {
        cerr << "creator socker error" << endl;
        return -1;
    }

    sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);// 主机字节序转换为网络字节序
    saddr.sin_addr.s_addr = inet_addr(ip);
    int res = connect(sock, (sockaddr *)&saddr, sizeof(saddr));
    if (res != 0) {
        cerr << "connect " << ip << " : " << port << " failed" << endl;
    }

    char buffer[1024] = {0};
    int len = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
        cout << buffer << endl;
    }

    strcpy(buffer, "send from client");
    len = send(sock, buffer, strlen(buffer), 0);

    close(sock);

    return 0;
}