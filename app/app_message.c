#include "app_message.h"
#include <stdlib.h>
#include "cjson/cJSON.h"
#include <string.h>
#include <stdio.h>
#include "app_common.h"

char *app_message_cjson2chars(char *cjson_str, int *str_len)
{
    // 将cjson字符串转换成cjson对象
    cJSON *root = cJSON_Parse(cjson_str);
    // 解析cJSON对象
    int conn_type = cJSON_GetObjectItem(root, "conn_type")->valueint;
    char *id_hex = cJSON_GetObjectItem(root, "id")->valuestring;
    char *msg_hex = cJSON_GetObjectItem(root, "msg")->valuestring;
    // 获取id的长度和msg的长度
    int id_len = strlen(id_hex)/2;
    int msg_len = strlen(msg_hex)/2;
    // 计算字符串的长度
    *str_len = id_len + msg_len + 3;
    // 将十六进制id和msg转换成字符串
    char *id_str = app_common_hex2chars(id_hex, *str_len);
    char *msg_str = app_common_hex2chars(msg_hex, *str_len);
    // 创建一个空的字符串
    char *str = malloc(*str_len);
    // 拼接字符串
    str[0] = conn_type;
    str[1] = id_len;
    str[2] = msg_len;
    memcpy(str + 3, id_str, id_len);
    memcpy(str + 3 + id_len, msg_str, msg_len);
    // 释放内存
    cJSON_Delete(root);
    free(id_str);
    free(msg_str);
    
    return str;
}

char *app_message_chars2cjson(char *chars)
{
    // 创建JSON对象
    cJSON *root = cJSON_CreateObject();
    // 解析字符串
    int conn_type = chars[0];
    int id_len = chars[1];
    int msg_len = chars[2];
    // 创建id和msg
    char id_hex[id_len];
    char msg_hex[msg_len];
    memcpy(id_hex, chars + 3, id_len);
    memcpy(msg_hex, chars + 3 + id_len, msg_len);
    // 将字符串转换成十六进制串
    char *id_str = app_common_chars2hex(id_hex, id_len);
    char *msg_str = app_common_chars2hex(msg_hex, msg_len);
    // 将字符串内容添加到JSON对象中
    cJSON_AddNumberToObject(root, "conn_type", conn_type);
    cJSON_AddStringToObject(root, "id", id_str);
    cJSON_AddStringToObject(root, "msg", msg_str);
    // 将JSON对象转换成字符串
    char *cjson = cJSON_PrintUnformatted(root);
    // 释放内存
    cJSON_Delete(root);
    free(id_str);
    free(msg_str);
    return cjson;
}
