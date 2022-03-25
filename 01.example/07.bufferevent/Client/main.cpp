#include <arpa/inet.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define SERVERPORT 8080
#define MAXBYTES 1024
#include <iostream>
using namespace std;

void *cmd_msg_cb(evutil_socket_t stdinfd, short what, void *arg)
{
  int ret;
  struct bufferevent *bev = (struct bufferevent *)arg;
  char buf[MAXBYTES];
  puts("get msg from stdin:");
  ret = read(stdinfd, buf, sizeof(buf));
  bufferevent_write(bev, buf, ret);
}

void read_buf_cb(struct bufferevent *bev, void *cbarg)
{
  cout << "read_buf_cb client" << endl;
  int ret;
  char buf[MAXBYTES];
  ret = bufferevent_read(bev, buf, sizeof(buf));
  write(STDOUT_FILENO, buf, ret);
}

void event_cb(struct bufferevent *bev, short event, void *cbarg)
{
  cout << "event_cb client" << endl;
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
    event_base_loopexit(base, NULL);
    // event_base_loopexit(bufferevent_get_base(bev), NULL);
  }
}

void main_loop(int clientfd)
{
  struct event_base *base;
  struct bufferevent *bev;
  struct event *ev_stdin;
  base = event_base_new();
  bev = bufferevent_socket_new(base, clientfd,
                               BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
  bufferevent_setcb(bev, (bufferevent_data_cb)read_buf_cb, NULL,
                    (bufferevent_event_cb)event_cb, (void *)base);
  bufferevent_enable(bev, EV_READ);
  bufferevent_enable(bev, EV_READ);
  ev_stdin = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, (event_callback_fn)cmd_msg_cb, (void *)bev);
  event_add(ev_stdin, NULL);
  event_base_dispatch(base);
  bufferevent_free(bev);
  event_free(ev_stdin);
  event_base_free(base);
  puts("exit now...");
}

int main(int argc, char **argv)
{
  int clientfd;
  struct sockaddr_in serveraddr;
  serveraddr.sin_family = AF_INET;
  const char *ip = "127.0.0.1";
  inet_pton(AF_INET, ip, &serveraddr.sin_addr.s_addr);
  serveraddr.sin_port = htons(SERVERPORT);
  clientfd = socket(AF_INET, SOCK_STREAM, 0);
  connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  main_loop(clientfd);
  return 0;
}
