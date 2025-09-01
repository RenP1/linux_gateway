#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__

#include "app_buffer.h"
#include <pthread.h>

// 设备的结构体
typedef struct
{
    char *filename;        // 设备文件（串口文件路径）
    int fd;                // 文件描述符 open(filename)产生
    Buffer *up_buffer;     // 上行缓冲区
    Buffer *down_buffer;   // 下行缓冲区
    pthread_t read_thread; // 读线程的标识
    int is_running;        // 读线程是否正在运行
    //读后处理函数  蓝牙数据 => 字符数组消息
    int (*post_read)(char* data,int len);
    //写前处理函数  字符数组消息 => 蓝牙数据
    int (*pre_write)(char* data,int len);
} Device;

//初始化模块
Device *app_device_init(char *filename);

//启动设备
int app_device_start(void);

//销毁设备
void app_device_destory(void);

#endif