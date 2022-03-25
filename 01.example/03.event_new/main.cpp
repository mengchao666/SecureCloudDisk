/*
 * @Author: your name
 * @Date: 2022-03-20 01:09:58
 * @LastEditTime: 2022-03-22 13:48:58
 * @LastEditors: Please set LastEditors
 * @Description: test libevent event_new
 * @FilePath: /code/libevent/src/main.cpp
 */
#include <iostream>
#include <event2/event.h>
#include <unistd.h>

using namespace std;

void StdinCallback(evutil_socket_t fd, short what, void *arg)
{
    char buffer[1024] = {0};
    cout << "stdin callback" << endl;
    cout << (char *)arg << endl;
    int ret = read(fd, buffer, sizeof(buffer));
    cout << buffer << endl;
}

int main1()
{
    char *ch1 = "ev";
    char *ch2 = "ev1";
    event_base *base = event_base_new();
    event *ev = event_new(base, STDIN_FILENO, EV_READ, StdinCallback, ch1);// 方式1传ev，方便回调函数event_del
    event_add(ev, NULL);// NULL表示没有超时时间，永远等待下去

    event *ev1 = event_new(base, STDIN_FILENO, EV_READ, StdinCallback, ch2);// 创建事件
    event_add(ev1, NULL);// NULL表示没有超时时间，永远等待下去 添加到事件的反应堆中

    /* eventloop */
    event_base_dispatch(base);// 激活一次循环就退出了



    event_base_free(base);
    event_free(ev);
    event_free(ev1);
    return 0;
}