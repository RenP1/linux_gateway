#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__

#include "app_buffer.h"
#include <pthread.h>

// 设备的结构体
typedef struct
{
    // 串口文件名
    char *filename;        
    // 文件描述符
    int fd;                
    // 上行缓冲区
    Buffer *up_buffer;     
    // 下行缓冲区
    Buffer *down_buffer;   
    // 读线程的标识符
    pthread_t read_thread; 
    // 读线程是否在运行
    int is_running;        
    //读后处理函数，蓝牙数据转换成字符串
    int (*post_read)(char* data,int len);
    //写前处理函数 ，字符串转换成蓝牙数据
    int (*pre_write)(char* data,int len);
} Device;

/**
 * @brief 初始化设备
 * 
 * @param filename 串口名
 * @return Device* 唯一设备
 */
Device *app_device_init(char *filename);

/**
 * @brief 设备启动
 * 
 * @return int 返回状态
 */
int app_device_start(void);

/**
 * @brief 销毁设备
 * 
 */
void app_device_destory(void);

#endif