#include "app_buffer.h"
#include <string.h>
#include <stdlib.h>
#include "log/log.h"
#include <pthread.h>

// 静态函数：初始化小buffer
static SubBuffer *sub_buffer_init(int size)
{
    // 申请空间
    SubBuffer *sub_buffer = (SubBuffer *)malloc(sizeof(SubBuffer));
    sub_buffer->ptr = (char *)malloc(size);
    sub_buffer->size = size;
    sub_buffer->len = 0;
    return sub_buffer;
}

// 静态函数：交换读缓冲区和写缓冲区
static void sub_buffer_swap(Buffer *buffer)
{
    log_debug("交互前,添加写锁");
    pthread_mutex_lock(&buffer->write_lock);
    int temp = buffer->read_buffer_index;
    buffer->read_buffer_index = buffer->write_buffer_index;
    buffer->write_buffer_index = temp;
    log_debug("交互后,释放写锁");
    pthread_mutex_unlock(&buffer->write_lock);
}

Buffer *app_buffer_init(int size)
{
    // 申请内存
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    // 初始化小buffer
    buffer->sub_buffer_list[0] = sub_buffer_init(size);
    buffer->sub_buffer_list[1] = sub_buffer_init(size);
    // 指定索引
    buffer->write_buffer_index = 0;
    buffer->read_buffer_index = 1;
    // 初始化锁
    pthread_mutex_init(&buffer->write_lock, NULL);
    pthread_mutex_init(&buffer->read_lock, NULL);
    // 返回buffer
    return buffer;
}

void app_buffer_free(Buffer *buffer)
{
    free(buffer->sub_buffer_list[0]->ptr);
    free(buffer->sub_buffer_list[0]);
    free(buffer->sub_buffer_list[1]->ptr);
    free(buffer->sub_buffer_list[1]);
    free(buffer);
}

int app_buffer_write(Buffer *buffer, char *data, int len)
{
    // 判断写入数据是否超过一个字节
    if (len > 255)
    {
        log_error("最多只能存储255长度数据!!!");
        return -1;
    }
    log_debug("添加写锁");
    pthread_mutex_lock(&buffer->write_lock);
    // 获取可写入的buffer
    SubBuffer *w_buffer = buffer->sub_buffer_list[buffer->write_buffer_index];
    // 判断是否还有空间写入数据
    if (w_buffer->len + len + 1 > w_buffer->size)
    {
        log_error("写入数据长度超过buffer大小!!!");
        log_debug("释放写锁");
        pthread_mutex_unlock(&buffer->write_lock);
        return -1;
    }
    // 写入数据
    w_buffer->ptr[w_buffer->len] = len;
    memcpy(w_buffer->ptr + w_buffer->len + 1, data, len);
    // 更新buffer长度
    w_buffer->len += len + 1;
    log_debug("释放写锁");
    pthread_mutex_unlock(&buffer->write_lock);
    return 0;
}

int app_buffer_read(Buffer *buffer, char *read_data, int buf_size)
{
    log_debug("添加读锁");
    pthread_mutex_lock(&buffer->read_lock);
    SubBuffer *r_buffer = buffer->sub_buffer_list[buffer->read_buffer_index];
    // 判断缓存是否为空
    if (r_buffer->len == 0)
    {
        // 互换缓冲区
        sub_buffer_swap(buffer);
        r_buffer = buffer->sub_buffer_list[buffer->read_buffer_index];
        // 判断缓存是否为空
        if (r_buffer->len == 0)
        {
            log_error("缓冲区中没有数据!!!");
            return -1;
        }
    }
    
    // 读取数据
    int len = r_buffer->ptr[0];
    // 判断容器大小是否满足
    if (len > buf_size)
    {
        log_error("容器大小不够!!!");
        return -1;
    }
    
    memcpy(read_data, r_buffer->ptr + 1, len);
    // 读完以后将数据前移
    memmove(r_buffer->ptr, r_buffer->ptr + len + 1, r_buffer->len - len - 1);
    // 更新数据长度
    r_buffer->len -= len + 1;
    log_debug("释放读锁");
    pthread_mutex_unlock(&buffer->read_lock);
    // 返回读取数据的长度
    return len;
}
