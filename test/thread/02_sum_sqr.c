#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int sqr_fct(int num)
{
    sleep(1);
    return num * num;
}

void *sqr_fctt(void *arg)
{
    int *num = (int *)arg;
    int *result = malloc(sizeof(int));
    *result = sqr_fct(*num);
    return result;
}

int main(int argc, char const *argv[])
{
    int nums[5] = {1, 2, 3, 4, 5};
    int sqr_nums[5];
    pthread_t tids[5];
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&tids[i], NULL, sqr_fctt, &nums[i]);
    }
    for (int i = 0; i < 5; i++)
    {
        void *result;
        pthread_join(tids[i], &result);
        sqr_nums[i] = *(int *)result;
    }
    gettimeofday(&end, NULL);
    printf("消耗时间：%ld s\n", end.tv_sec - start.tv_sec);
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        sum += sqr_nums[i];
    }
    printf("sum = %d\n", sum);
    return 0;
}
