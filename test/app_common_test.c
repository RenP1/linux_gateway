#include "app_common.h"
#include "log/log.h"

int main(int argc, char const *argv[])
{
    long time = app_common_get_timestamp();
    log_info("time: %ld", time);
    return 0;
}
