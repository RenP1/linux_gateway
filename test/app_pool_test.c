#define _GNU_SOURCE
#include "app_pool.h"
#include <unistd.h>
#include <stdio.h>

int task_fun(void *arg)
{
    int i = (int)arg;
    printf("hello_%d,id_%d \n",i,gettid());
    return 0;
}

int main(int argc, char const *argv[])
{
    // 初始化pool
    app_pool_init(5);
    for (int i = 0; i < 5; i++)
    {
        // 注册任务
        app_pool_register_task(task_fun,(void *)i);
    }
    // 休眠1s
    sleep(1);
    // 销毁pool
    app_pool_destroy();
    return 0;
}
