#ifndef __APP_BUFFER_H__
#define __APP_BUFFER_H__

#include <pthread.h>

// 小缓冲区
typedef struct
{
    unsigned char *ptr; // 小缓冲区指针
    int size;  // 小缓冲区大小
    int len;   // 小缓冲区已存储数据的长度
} SubBuffer;

// 缓冲区
typedef struct
{
    SubBuffer *sub_buffers[2];  // 包含读和写缓冲区的数组
    int read_index;             // 读缓冲区索引
    int write_index;            // 写缓冲区索引
    pthread_mutex_t read_lock;  // 读锁
    pthread_mutex_t write_lock; // 写锁
} Buffer;

// 初始化
Buffer *app_buffer_init(int size);

// 释放
void app_buffer_free(Buffer *buffer);

// 写入数据
int app_buffer_write(Buffer *buffer, char *data, int len);

// 读取数据
int app_buffer_read(Buffer *buffer, char *data_buf, int buf_size);

#endif