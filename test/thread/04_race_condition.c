#include <pthread.h>
#include <stdio.h>

// 初始化互斥锁
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *sum_fun(void *arg)
{
    int *p = (int *)arg;
    // 加锁
    pthread_mutex_lock(&mutex);
    *p = *p + 1;
    // 解锁
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char const *argv[])
{
    int num = 0;
    pthread_t threads[20000];
    for (int i = 0; i < 20000; i++)
    {
        pthread_create(&threads[i], NULL, (void *)sum_fun, &num);
    }
    for (int i = 0; i < 20000; i++)
    {
        pthread_join(threads[i], NULL);
    }
    // 销毁锁
    pthread_mutex_destroy(&mutex);
    printf("num = %d\n", num);
    return 0;
}
