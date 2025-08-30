#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// 创建消息队列描述符
static mqd_t mq_fd;

// 发送消息
void *frist_thread(void *arg)
{
    printf("第一个进程执行，准备发送消息\n");
    char *str = "hello world";
    // 发送消息
    int send = mq_send(mq_fd, str, strlen(str)+1, 1);
    if (send == -1)
    {
        printf("发送消息失败\n");
    }
}

// 接收消息
void *second_thread(void *arg)
{
    sleep(1);
    printf("第二个进程执行，准备接收消息\n");
    // 接收消息
    char buf[100];
    ssize_t receive = mq_receive(mq_fd, buf, 100, NULL);
    if (receive == -1)
    {
        printf("接收消息失败\n");
    }
    
    printf("接收到的消息是：%s\n", buf);
}

int main(int argc, char const *argv[])
{
    // 设置消息队列属性
    struct mq_attr attr;
    // 设置消息队列最大消息长度
    attr.mq_maxmsg = 10;
    // 设置消息队列每个消息最大长度
    attr.mq_msgsize = 100;
    // 创建消息队列
    mq_fd = mq_open("/mq_test", O_CREAT | O_RDWR, 0644, &attr);
    pthread_t t1, t2;
    // 创建线程
    pthread_create(&t1, NULL, frist_thread, NULL);
    pthread_create(&t2, NULL, second_thread, NULL);
    // 等待线程结束
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    // 删除消息队列
    mq_close(mq_fd);
    mq_unlink("/mq_test");
    return 0;
}
