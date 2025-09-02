#include "app_buffer.h"
#include "log/log.h"

int main(int argc, char const *argv[])
{
    // 初始化缓冲区
    Buffer *buffer = app_buffer_init(1024);
    // 向缓冲区写入数据
    app_buffer_write(buffer, "theshy", 6);
    app_buffer_write(buffer, "rookie", 6);
    // 读取缓冲区数据
    char str1[15];
    int str1_len = app_buffer_read(buffer, str1, 15);
    log_debug("读取到数据:%.*s,长度为%d", str1_len, str1, str1_len);
    // 读取缓冲区数据
    char str2[15];
    int str2_len = app_buffer_read(buffer, str2, 15);
    log_debug("读取到数据:%.*s,长度为%d", str2_len, str2, str2_len);
    // 读取缓冲区数据
    // char str3[15];
    // int str3_len = app_buffer_read(buffer, str3, 15);
    // log_debug("读取到数据:%.*s,长度为%d", str3_len, str3, str3_len);

    // 释放缓冲区
    app_buffer_free(buffer);
    return 0;
}
