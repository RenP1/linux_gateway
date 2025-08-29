#ifndef __APP_COMMON_H__
#define __APP_COMMON_H__ 

/**
 * @brief 获取当前时间戳(毫秒数)
 * 
 * @return long 时间戳
 */
long app_common_get_timestamp(void);

/**
 * @brief 将字符串转换成16进制字符串
 * 
 * @param chars 要转换的字符串
 * @param chars_len 要转换的字符串长度
 * @return char* 16进制字符串
 */
char *app_common_chars2hex(char *chars, int chars_len);

/**
 * @brief 将16进制字符串转换成字符串
 * 
 * @param hex 要转换的16进制字符串
 * @param hex_len 要转换的16进制字符串长度
 * @return char* 字符串
 */
char *app_common_hex2chars(char *hex, int hex_len);

#endif