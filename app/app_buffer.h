#ifndef __APP_BUFFER_H__
#define __APP_BUFFER_H_

#include <pthread.h>

// sub_buffer结构体 
typedef struct
{
    unsigned char *ptr; // 缓冲区指针
    int size; // 缓冲区大小
    int len;   // 缓冲区数据长度
}SubBuffer;

// 整个缓冲区结构体
typedef struct
{
    SubBuffer *sub_buffer_list[2]; // 两个小buffer
    int write_buffer_index; // 读缓冲区索引
    int read_buffer_index; // 写缓冲区索引
    pthread_mutex_t write_lock; // 写缓冲区锁
    pthread_mutex_t read_lock; // 读缓冲区锁
}Buffer;

/**
 * @brief 初始化缓冲区
 * 
 * @param size 缓冲区大小
 * @return Buffer* 唯一缓冲区
 */
Buffer *app_buffer_init(int size);

/**
 * @brief 释放缓冲区
 * 
 * @param buffer 指定的缓冲区
 */
void app_buffer_free(Buffer *buffer);

/**
 * @brief 向缓冲区写入数据
 * 
 * @param buffer 指定的缓冲区
 * @param data 写入数据
 * @param len  写入数据长度
 * @return int 返回状态，-1 失败，0成功
 */
int app_buffer_write(Buffer *buffer, char *data, int len);

/**
 * @brief 从缓冲区读取数据
 * 
 * @param buffer 指定的缓冲区
 * @param read_data 存储读取数据的buf
 * @param buf_size 存储读取数据的buf大小
 * @return int 返回读取数据长度
 */
int app_buffer_read(Buffer *buffer, char *read_data, int buf_size);

#endif