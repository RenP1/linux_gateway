#include "app_device.h"
#include "fcntl.h"
#include "app_buffer.h"
#include "app_pool.h"
#include "app_mqtt.h"
#include "log/log.h"
#include "unistd.h"
#include "app_message.h"
#include "stdlib.h"

#define BUFFER_SIZE 1024
#define POOL_SIZE 5

static Device *device = NULL;

Device *app_device_init(char *filename)
{
    // 保证只产生一个设备
    if (device)
    {
        return device;
    }
    // 申请设备内存
    device = (Device *)malloc(sizeof(Device));
    // 初始化其属性
    device->filename = filename;
    device->fd = open(filename, O_RDWR);
    device->up_buffer = app_buffer_init(BUFFER_SIZE);
    device->down_buffer = app_buffer_init(BUFFER_SIZE);
    device->is_running = 0;
    device->post_read=NULL;
    device->pre_write=NULL;

    //初始化线程池模块
    app_pool_init(POOL_SIZE);

    //初始化mqtt模块
    app_mqtt_init();

    return device;
}


// 在线程池中某个分线程执行的任务函数
static int send_task_fun(void *arg)
{
    // 从上行缓冲区读取消息数据
    char data_buf[128];
    int len = app_buffer_read(device->up_buffer, data_buf, sizeof(data_buf));
    if (len <= 0)
    {
        log_error("读取上行缓冲区数据失败");
        return -1;
    }

    // 将字符数组数据转换为json消息
    char *json = app_message_charsToJson(data_buf, len);

    // 将json消息发送到远程
    int result=app_mqtt_send(json);
    if(result==-1)
    {
        log_error("发送消息失败");
        free(json);
        return -1;
    }

    free(json);
    log_debug("发送消息成功：%s",json);
    return 0;
}

/**
 * @brief 不断读取串口数据的线程函数
 *
 * @return void*
 */
static void *read_thread_fun(void *arg)
{
    while (device->is_running)
    {
        // 读取串口数据
        char data_buf[128];
        int len = read(device->fd, data_buf, sizeof(data_buf));
        log_debug("读取数据长度:%d",len);
        if (len > 0)
        {
            //将读取的数据处理成字符数组消息
            len=device->post_read(data_buf,len);
        }
        if(len>0)
        {
            //将字符数组保存到上行缓冲区
            app_buffer_write(device->up_buffer,data_buf,len);
            //注意任务：将发送消息到远程的操作交给线程池
            app_pool_registerTask(send_task_fun,NULL);
        }
    }
}


int app_device_start(void)
{
    if (device->is_running)
    {
        log_debug("设备已经正在运行中...");
        return 0;
    }
    device->is_running = 1;
    // 创建读线程
    pthread_create(&device->read_thread, NULL, read_thread_fun, NULL);

    return 0;
}

void app_device_destory(void)
{
    close(device->fd);
    app_buffer_free(device->up_buffer);
    app_buffer_free(device->down_buffer);
    pthread_cancel(device->read_thread);
    device->is_running = 0;
    device->post_read = NULL;
    device->pre_write = NULL;
    app_pool_destory();
    app_mqtt_close();
}