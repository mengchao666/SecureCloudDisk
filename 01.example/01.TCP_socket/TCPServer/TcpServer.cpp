/*
 * @Author: mengchaobbbigrui
 * @Date: 2022-03-20 19:45:38
 * @LastEditTime: 2022-03-20 23:03:21
 * @LastEditors: Please set LastEditors
 * @Description: TcpServer.cpp
 * @FilePath: /code/SecureCloudDisk/TCP/TCPServer/TcpServer.cpp
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
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    // 1.create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock <= 0) {
        cerr << "creator socker error" << endl;
        return -1;
    }
    cout << "sock = " << sock << endl;

    // bind port
    sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);// 主机字节序转换为网络字节序
    saddr.sin_addr.s_addr = htonl(0);
    if (::bind(sock, (sockaddr *)&saddr, sizeof(saddr)) != 0) {
        cerr << "bind port " << port << " failed!"<< endl; 
    }
    cout << "bind port " << port << " success" << endl;

    // listen
    listen(sock, 10);

    // accept
    sockaddr_in caddr;// 客户端地址
    socklen_t len = 0;
    int client_sock = accept(sock, (sockaddr*)&caddr, &len);
    cout << "client sock = " << client_sock << endl;

    //send
    char buffer[1024] = "welcome to xms";
    int length = send(client_sock, buffer, strlen(buffer), 0);
    cout << "length = " << length << endl;

    //recv
    length = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    if (length > 0) {
        buffer[length] = '\0';
        cout << buffer << endl;
    }

    close(sock);

    return 0;
}