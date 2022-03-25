/*
 * @Author: mengchaobbbigrui
 * @Date: 2022-03-20 19:45:38
 * @LastEditTime: 2022-03-20 23:03:21
 * @LastEditors: mengchaobbbigrui
 * @Description: TcpServer.cpp
 * @FilePath: /code/SecureCloudDisk/TCP/TCPServer/TcpServer.cpp
 */
#include <arpa/inet.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#define PORT 8080

using namespace std;

void read_buf_cb(struct bufferevent *bev, void *cbarg)
{
    int ret, i;
    char buf[1024];
    ret = bufferevent_read(bev, buf, sizeof(buf));
    printf("read_buf_cd length %d\n", ret);
    for (i = 0; i < ret; i++)
    {
        buf[i] = toupper(buf[i]);
    }
    bufferevent_write(bev, buf, ret);
}

void event_cb(struct bufferevent *bev, short event, void *cbarg)
{
    cout << "event_cb" << endl;
    struct event_base *base = (struct event_base *)cbarg;
    if (BEV_EVENT_READING & event)
        puts("BEV_EVENT_READING");
    if (BEV_EVENT_WRITING & event)
        puts("BEV_EVENT_WRITING");
    if (BEV_EVENT_ERROR & event)
        puts("BEV_EVENT_ERROR");
    if (BEV_EVENT_EOF & event)
    {
        puts("BEV_EVENT_EOF");
        bufferevent_free(bev);
    }
}

void AcceptCb(evutil_socket_t sock, short what, void *arg)
{
    cout << "AcceptCb" << endl;
    sockaddr_in clientAddr;
    socklen_t clientAddrLen;
    bufferevent *bev;
    event_base *base = (struct event_base *)arg;
    cout << "accept is here" << endl;
    bzero((void *)&clientAddr, sizeof(clientAddr));
    int clientFd = accept(sock, (struct sockaddr *)&clientAddr, &clientAddrLen);
    cout << "clientFd = " << clientFd << endl;
    evutil_make_socket_nonblocking(clientFd);
    bev = bufferevent_socket_new(
        base, clientFd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
    bufferevent_setcb(bev, (bufferevent_data_cb)read_buf_cb, NULL,
                      (bufferevent_event_cb)event_cb, (void *)base);
    bufferevent_enable(bev, EV_READ);
}

void MainLoop(int sock)
{
    cout << "MainLoop" << endl;
    event_base *base = event_base_new();
    event *ev = event_new(base, sock, EV_READ | EV_PERSIST,
                          (event_callback_fn)AcceptCb, (void *)base);
    event_add(ev, NULL);

    event_base_dispatch(base);
    event_free(ev);
    event_base_free(base);
}

int main(int argc, char **argv)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock <= 0)
    {
        cerr << "create socker error" << endl;
        return -1;
    }
    cout << "sock = " << sock << endl;

    // bind port
    sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT); // 主机字节序转换为网络字节序
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(sock, (sockaddr *)&saddr, sizeof(saddr)) != 0)
    {
        cerr << "bind port " << PORT << " failed!" << endl;
    }
    cout << "bind port " << PORT << " success" << endl;

    // listen
    listen(sock, 10);

    MainLoop(sock);

    close(sock);

    return 0;
}
