#include "app_pool.h"
#include "mqueue.h"
#include "pthread.h"
#include <string.h>
#include <stdlib.h>

static mqd_t mq_fd;
static char *mq_name = "/app_pool_mqueque";
static int thread_num = 0;
static pthread_t *threads;

void *thread_fun(void *arg)
{
    Task task;
    // 循环读取任务队列，如果接收不到就阻塞
    while (1)
    {
        // 从任务队列中读取任务
        mq_receive(mq_fd, (char *)&task, sizeof(task), 0);
        task.task(task.arg);
    }
}

int app_pool_init(int thread_count)
{
    // 配置任务队列属性
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(Task);
    // 创建任务(消息)队列
    mq_fd = mq_open(mq_name, O_CREAT | O_RDWR, 0644, &attr);
    // 创建线程池
    thread_num = thread_count;
    threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_num);
    // 创建线程
    for (int i = 0; i < thread_num; i++)
    {
        pthread_create(threads + i, NULL, thread_fun, NULL);
    }
    return 0;
}

void app_pool_destroy(void)
{
    // 关闭任务队列
    mq_close(mq_fd);
    // 删除队列消息
    mq_unlink(mq_name);
    // 关闭进程
    for (int i = 0; i < thread_num; i++)
    {
        pthread_cancel(threads[i]);
        pthread_join(threads[i],NULL);
    }
    // 释放线程池
    free(threads);
    
}

int app_pool_register_task(int (*task_fun)(void *arg), void *arg)
{
    // 创建结构体接收参数
    Task task = {
        .task = task_fun,
        .arg = arg,
    };
    // 发送消息
    return mq_send(mq_fd, (char *)&task, sizeof(Task), 0);
}
