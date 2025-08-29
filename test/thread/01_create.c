#include <pthread.h>
#include <stdio.h>

void *fristthread_fun(void *arg){
    printf("第一个进程(%d)执行...\n",gettid());
}

void *secondthread_fun(void *arg){
    printf("第二个进程(%d)执行...\n",gettid());
}

int main(int argc, char const *argv[])
{
    printf("主进程执行...\n");
    pthread_t p1,p2;
    pthread_create(&p1,NULL,fristthread_fun,NULL);
    pthread_create(&p2,NULL,secondthread_fun,NULL);
    printf("主程序(%d)继续执行...\n",gettid());
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    return 0;
}
