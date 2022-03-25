/*
 * @Author: your name
 * @Date: 2022-03-20 01:09:58
 * @LastEditTime: 2022-03-22 00:50:56
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /code/libevent/src/main.cpp
 */
#include <iostream>
#include <event2/event.h>

using namespace std;


// 新建event_base_new
int main1()
{
    cout << "libevent test" << endl;

    // 创建libevent上下文
    event_base *base = event_base_new();// 默认使用默认的后端epoll，且不可更改
    //  想使用自定义的后端，只能用event_base_new_with_config
    if (base) {
        cout << "event_base_new success" << endl;
    }
    cout << "default base " << event_base_get_method(base) << endl;// 查看当前操作系统的默认后端是什么，Linux默认为epoll
    event_base_free(base);
    cout << "quit" << endl;
    return 0;
}

// 尝试使用event_config配置event_base,此时只能使用event_base_new_with_config,而不能使用event_base_new()
// 同时尝试，使用event_config_aviod_method(),禁用特定后端
int main2()
{
    event_config *config = event_config_new();
    event_config_avoid_method(config, "epoll"); // 禁用epoll
    // event_config_avoid_method(config, "poll");
    // event_config_avoid_method(config, "select");
    event_base *base = event_base_new_with_config(config);

    cout << "get current type " << event_base_get_method(base) << endl;

    event_base_free(base);
    event_config_free(config);
}

// 尝试使用event_config配置event_base,此时只能使用event_base_new_with_config,而不能使用event_base_new()
// 同时尝试，使用event_config_require_features(),使用特定后端
int main3()
{
    event_config *config = event_config_new();
    // event_config_require_features(config, EV_FEATURE_ET);// 配置指定的模式 EV_FEATURE_ET为epoll专用模式，所以这里会配置成epoll

    event_config_require_features(config, EV_FEATURE_ET | EV_FEATURE_O1);// O1也是只有epoll支持，所以这里还是会配置成epoll
    event_base *base = event_base_new_with_config(config);
    cout << "get current type " << event_base_get_method(base) << endl;

    event_base_free(base);
    event_config_free(config);
}

// 尝试event_config_set_flag(config, event_base_config_flag),配置config
int main()
{
    event_config *config = event_config_new();
    event_config_set_flag(config, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);
    
    event_base *base = event_base_new_with_config(config);
    cout << "get current type " << event_base_get_method(base) << endl;

    event_base_free(base);
    event_config_free(config);
}
