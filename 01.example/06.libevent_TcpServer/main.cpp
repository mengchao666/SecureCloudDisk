#include <iostream>
#include <event2/event.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

using namespace std;

#define SERVER_PORT 8080

void ReadCB(evutil_socket_t fd, short what, void *arg)
{
    cout << "READCB func start" << endl;
    char buffer[1024] = "welcome to xms";
    int length = send(fd, buffer, strlen(buffer), 0);
    cout << "length = " << length << endl;
    length = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (length > 0) {
        buffer[length] = '\0';
        cout << buffer << endl;
    }
    cout << "READCB func end" << endl;
    close(fd);
}

void AcceptCB(evutil_socket_t fd, short what, void *arg)
{
    event_base *base = (event_base *)arg;
    sockaddr_in clientAddr;
    socklen_t len = 0;
    int clientFd = accept(fd, (sockaddr*)&clientAddr, &len);
    cout << "clientFd = " << clientFd << endl;
    event *clientEv = event_new(base, clientFd, EV_READ | EV_WRITE | EV_PERSIST | EV_ET, ReadCB, NULL);
    event_add(clientEv, NULL);
}

void MainLoop(int fd)
{
    event_base *base = event_base_new();
    event *listen = event_new(base, fd, EV_READ | EV_WRITE | EV_PERSIST | EV_ET, AcceptCB, base);

    event_add(listen, NULL);
    event_base_dispatch(base);

    event_del(listen);
    event_free(listen);
    event_base_free(base);
}

int main()
{
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(SERVER_PORT);
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (::bind(fd, (sockaddr *)&saddr,sizeof(saddr)) != 0) {
        cout << "bind error " << endl;
    }
    cout << "bind port " << SERVER_PORT << " success" << endl;


    listen(fd, 10);

    MainLoop(fd);
    return 0;

}