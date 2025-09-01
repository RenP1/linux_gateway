#ifndef __APP_POOL_H__
#define __APP_POOL_H__ 

// 创建任务结构体
typedef struct
{
    int (*task)(void *arg);
    void *arg;
}Task;

/**
 * @brief 启动线程池
 * 
 * @param thread_count 线程数量
 * @return int 返回0表示成功，1表示失败
 */
int app_pool_init(int thread_count);

/**
 * @brief 销毁线程池
 * 
 */
void app_pool_destroy(void);

/**
 * @brief 注册函数到任务队列
 * 
 * @param task 函数
 * @param arg 函数参数
 */
int app_pool_register_task(int (*task_fun)(void *arg), void *arg);

#endif