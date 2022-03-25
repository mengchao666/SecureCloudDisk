/*
 * @Author: your name
 * @Date: 2022-03-20 01:09:58
 * @LastEditTime: 2022-03-21 23:38:42
 * @LastEditors: Please set LastEditors
 * @Description: test libevent is install success
 * @FilePath: /code/libevent/src/main.cpp
 */
#include <iostream>
#include "event2/event.h"

using namespace std;

int main()
{
    cout << "libevent test" << endl;

    cout << "version = " << event_get_version() << endl; // 查看libevent版本

    const char ** ptr = event_get_supported_methods(); // 查看支持的方法 epoll select poll
    int i = 0;
    while (ptr[i]) {
        cout << "event method " << ptr[i] << endl;
        i++;
    }

    // 创建libevent上下文
    event_base *base = event_base_new();
    if (base) {
        cout << "event_base_new success" << endl;
    }
    cout << "quit" << endl;
    return 0;
}
