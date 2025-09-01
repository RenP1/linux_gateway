#include "app_buffer.h"
#include <stdlib.h>
#include <string.h>
#include "log/log.h"

// 初始化生成小缓冲区
static SubBuffer *init_sub_buffer(int size)
{
    SubBuffer *sub_buffer = (SubBuffer *)malloc(sizeof(SubBuffer));
    sub_buffer->ptr = (char *)malloc(size);
    sub_buffer->size = size;
    sub_buffer->len = 0;

    return sub_buffer;
}

// 初始化缓冲区
Buffer *app_buffer_init(int size)
{
    // 申请内存
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));

    // 初始化内部属性
    buffer->sub_buffers[0] = init_sub_buffer(size);
    buffer->sub_buffers[1] = init_sub_buffer(size);
    buffer->read_index = 0;
    buffer->write_index = 1;

    //初始化创建读锁和写锁
    pthread_mutex_init(&buffer->read_lock,NULL);
    pthread_mutex_init(&buffer->write_lock,NULL);

    return buffer;
}

// 释放
void app_buffer_free(Buffer *buffer)
{
    //先释放小缓冲区的内存，再释放小缓冲区的结构体
    free(buffer->sub_buffers[0]->ptr);
    free(buffer->sub_buffers[0]);
    free(buffer->sub_buffers[1]->ptr);
    free(buffer->sub_buffers[1]);
    //再释放总缓冲区
    free(buffer);
}

// 写入数据
int app_buffer_write(Buffer *buffer, char *data, int len)
{
    // 判断数据长度是否合法（不能超过255 => 下面会用一个字节来存储这个len）
    if (len > 255)
    {
        log_error("数据长度不合法,不能超过255...");
        return -1;
    }

    //加写锁
    log_debug("加写锁...");
    pthread_mutex_lock(&buffer->write_lock);

    // 得到写的小buffer
    SubBuffer *w_buffer = buffer->sub_buffers[buffer->write_index];

    // 判断剩余空间是否足够
    if (w_buffer->size - w_buffer->len < len + 1)
    {
        log_error("buffer剩余空间不足...");
        //解写锁
        pthread_mutex_unlock(&buffer->write_lock);
        return -1;
    }

    // 向小buffer中写入数据长度和数据
    w_buffer->ptr[w_buffer->len] = len;
    memcpy(w_buffer->ptr + w_buffer->len + 1, data, len);

    // 更新小buffer中的len
    w_buffer->len += len + 1;

    //解写锁
    log_debug("解写锁...");
    pthread_mutex_unlock(&buffer->write_lock);

    return 0;
}

//交换读写小缓冲区的下标
//如果正在写，不能交互
void swap_buf_bufer(Buffer *buffer)
{
    //加写锁
    log_debug("交互前，加写锁...");
    pthread_mutex_lock(&buffer->write_lock);
    int tmp = buffer->read_index;
    buffer->read_index = buffer->write_index;
    buffer->write_index = tmp;
    //解写锁
    log_debug("交互后，解写锁...");
    pthread_mutex_unlock(&buffer->write_lock);
}

// 读取数据
int app_buffer_read(Buffer *buffer, char *data_buf, int buf_size)
{
    // 加读锁
    log_debug("加读锁");
    pthread_mutex_lock(&buffer->read_lock);

    // 得到读的小buffer
    SubBuffer *r_buffer = buffer->sub_buffers[buffer->read_index];
    // 如果当前读buffer为空，则切换一下小缓冲区
    if (r_buffer->len == 0) {
        swap_buf_bufer(buffer);
        r_buffer = buffer->sub_buffers[buffer->read_index];
        // 如果交互后还是空的，则说明没有数据，失败返回-1
        if (r_buffer->len == 0) {
            log_error("buffer中没有数据");
            return -1;
        }
    }

    // 读取数据长度
    int len = r_buffer->ptr[0];

    // 判断容器大小是否足够
    if (buf_size < len) {
        log_error("容器大小不够");
        return -1;
    }

    // 读取对应的数据，保存到data_buf
    memcpy(data_buf, r_buffer->ptr+1, len);
    // 将后面未读取的数据全部移到到起始位置
    memmove(r_buffer->ptr, r_buffer->ptr+len+1, r_buffer->size-len-1);

    // 更新小buffer中的len
    r_buffer->len -= len + 1;

    // 解读锁
    log_debug("解读锁");
    pthread_mutex_unlock(&buffer->read_lock);

    // 返回读取的数据长度
    return len;
}