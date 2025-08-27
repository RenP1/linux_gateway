#include "app_common.h"
#include <sys/time.h>
#include <stddef.h>

long app_common_get_timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // tv_sec 秒数的时间，tv_usec 描述的微秒数
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}