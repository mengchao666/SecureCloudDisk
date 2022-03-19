/*
 * @Author: your name
 * @Date: 2022-03-20 01:09:58
 * @LastEditTime: 2022-03-20 01:14:35
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /code/libevent/src/main.cpp
 */
#include <iostream>
#include <event2/event.h>

using namespace std;

int main()
{
    cout << "libevent test" << endl;

    // 创建libevent上下文
    event_base *base = event_base_new();
    if (base) {
        cout << "event_base_new success" << endl;
    }
    cout << "quit" << endl;
    return 0;
}