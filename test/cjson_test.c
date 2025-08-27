#include "cjson/cJSON.h"
#include "log/log.h"

int main(int argc, char const *argv[])
{
    // 生成一个cJSON对象
    cJSON *cjson_obj1 = cJSON_CreateObject();
    // 向cJSON对象中添加属性
    cJSON_AddStringToObject(cjson_obj1, "name", "zhangsan");
    cJSON_AddNumberToObject(cjson_obj1, "age", 18);
    // 将cJSON对象转换成字符串
    char *json_str = cJSON_PrintUnformatted(cjson_obj1);
    // 打印字符串
    log_info("转换成cjson字符串: %s\n", json_str);

    // 解析字符串
    cJSON *cjson_obj2 = cJSON_Parse(json_str);
    // 获取属性值
    cJSON *cjson_name = cJSON_GetObjectItem(cjson_obj2, "name");
    cJSON *cjsonage = cJSON_GetObjectItem(cjson_obj2, "age");
    if (cjson_name == NULL || cjsonage == NULL)
    {
        log_info("属性值不存在\n");
        return -1;
    }
    
    // 打印属性值
    log_info("解析出来cJSON属性值 name: %s, age: %d\n", cjson_name->valuestring, cjsonage->valueint);
    // 释放cJSON对象
    cJSON_Delete(cjson_obj1);
    cJSON_Delete(cjson_obj2);
    free(json_str);
    return 0;
}
