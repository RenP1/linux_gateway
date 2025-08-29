#include "app_message.h"
#include "log/log.h"


int main(int argc, char const *argv[])
{
    char *json_str = "{\"conn_type\":1,\"id\":\"5858\",\"msg\":\"61626364\"}";
    int chars_len = 0;
    char *chars = app_message_cjson2chars(json_str,&chars_len);
    log_info("chars: %s, len: %d",chars, chars_len);
    char *json_str2 = app_message_chars2cjson(chars);
    log_info("json_str2: %s",json_str2);
    return 0;
}
