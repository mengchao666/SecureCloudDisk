/*
 * @Author: your name
 * @Date: 2022-03-20 01:09:58
 * @LastEditTime: 2022-03-22 19:09:34
 * @LastEditors: Please set LastEditors
 * @Description: set event priority and event loop
 * @FilePath: /code/libevent/src/main.cpp
 */
#include <iostream>
#include <event2/event.h>
#include <unistd.h>

using namespace std;

// 设置事件的优先级，0最高
#if 0
void StdinCallback2(evutil_socket_t fd, short what, void *arg)
{
    char buffer[1024] = {0};
    cout << (char *)arg << endl;
    cout << "stdin callback" << endl;
    int ret = read(fd, buffer, sizeof(buffer));
    cout << buffer << endl;
}

// 回调函数设置退出
int main()
{
    char *ch1 = "ev";
    char *ch2 = "ev1";
    event_base *base = event_base_new();
    event_base_priority_init(base, 4);
    event *ev = event_new(base, STDIN_FILENO, EV_READ, StdinCallback2, ch1);// 创建事件
    event_priority_set(ev, 3);
    event_add(ev, NULL);// NULL表示没有超时时间，永远等待下去

    event *ev1 = event_new(base, STDIN_FILENO, EV_READ, StdinCallback2, ch2);// 创建事件
    event_priority_set(ev1, 1);
    event_add(ev1, NULL);// NULL表示没有超时时间，永远等待下去

    /* eventloop */
    event_base_dispatch(base);// 激活一次循环就退出了



    event_base_free(base);
    event_free(ev);
    event_free(ev1);
    return 0;
}
#endif

void StdinCallback2(evutil_socket_t fd, short what, void *arg)
{
    static int time = 0;
    char buffer[1024] = {0};
    event_base *base = (event_base *)arg;
    cout << "stdin callback" << endl;
    int ret = read(fd, buffer, sizeof(buffer));
    cout << buffer << endl;
    // 循环大于n次，结束监听
    if (time > 5) {
        event_base_loopbreak(base);
        // event_base_loopexit();// 不想要超时时间，可以写NULL，这个函数是所有处于激活状态都处理完的时候，才会退出
    }
    time++;
}

// 回调函数设置退出
int main()
{
    char *ch1 = "ev";
    char *ch2 = "ev1";
    event_base *base = event_base_new();
    event_base_priority_init(base, 4);
    event *ev = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, StdinCallback2, base);// 创建事件
    event_priority_set(ev, 3);
    event_add(ev, NULL);// NULL表示没有超时时间，永远等待下去

    event *ev1 = event_new(base, STDIN_FILENO, EV_READ | EV_PERSIST, StdinCallback2, base);// 创建事件
    event_priority_set(ev1, 1);
    event_add(ev1, NULL);// NULL表示没有超时时间，永远等待下去

    /* eventloop */
    // event_base_loop(base, EVLOOP_ONCE);// 激活一次循环就退出了,尽管事件是EV_PERSIST持续性的,也会是只有一次
    // event_base_loop(base, EVLOOP_NONBLOCK); // 非阻塞性,检查一遍，直接退出
    event_base_dispatch(base);


    // base之前可以看base是怎么退出的
    // event_base_got_break(base); event_base_got_exit(base);

    event_base_free(base);
    event_free(ev);
    event_free(ev1);
    return 0;
}