#include <pthread.h>

// 长时间执行的线程
void *task(void *arg)
{
    printf("task线程开始执行\n");

    // 模拟工作
    printf("task线程工作中...\n");
    sleep(3);

    printf("task线程sleep之后执行结束\n");

    return NULL;
}

int main(int argc, char const *argv[])
{
    printf("主线程开始\n");
    pthread_t pt;
    // 创建线程
    pthread_create(&pt, NULL, task, NULL);
    // 等待一段时间 =》 让分线程先执行
    sleep(1);

    // 取消分线程
    pthread_cancel(pt);
    // 等待分线程结束
    void *result;
    pthread_join(pt, &result);

    // 检查分线程是否被取消
    if (result == PTHREAD_CANCELED)
    {
        printf("分线程被取消结束\n");
    }
    else
    {
        printf("分线程正常结束\n");
    }

    printf("主线程结束\n");
    return 0;
}
