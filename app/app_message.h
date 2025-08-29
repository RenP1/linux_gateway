#ifndef __APP_MESSAGE_H__
#define __APP_MESSAGE_H__ 

/**
 * @brief 将接收到的cjson消息字符串转换成字符数组
 * 
 * @param cjson_str 接收到的cjson消息字符串
 * @param len 转换好的字符数组的长度
 * @return char* 转换好的字符数组
 */
char *app_message_cjson2chars(char *cjson_str,int *len);

/**
 * @brief 将字符数组转换成cjson消息字符串
 * 
 * @param chars 字符数组
 * @param len 字符数组的长度
 * @return char* 转换好的cjson消息字符串
 */
char *app_message_chars2cjson(char *chars);

#endif