#include "app_common.h"
#include <sys/time.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

long app_common_get_timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // tv_sec 秒数的时间，tv_usec 描述的微秒数
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

char *app_common_chars2hex(char *chars, int chars_len)
{
    // 创建内存空间
    char *hex = malloc(chars_len * 2 + 1);
    // 将字符串转换成十六进制
    for (int i = 0; i < chars_len; i++)
    {
        sprintf(hex + i * 2, "%02x", chars[i]);
    }
    // 添加字符串结束符
    hex[chars_len * 2] = '\0';
    return hex;
}

char *app_common_hex2chars(char *hex, int hex_len)
{
    // 创建内存空间
    char *str = malloc(hex_len / 2 + 1);
    // 遍历hexstr中的每个字符(以2个为单位)，得到对应的字符，并保存到chars中
    for (int i = 0; i < hex_len; i+=2)
    {
        sscanf(hex + i, "%02hhx", &str[i / 2]);
    }
    return str;
}
