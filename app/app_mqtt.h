#ifndef __APP_MQTT_H__
#define __APP_MQTT_H__

#define ADDRESS "tcp://192.168.12.59:1883"
#define CLIENTID "148a9d6c-c0da-4ece-9fbf-2d6e732e1687"
#define TOPIC_GTR "gateway_to_remote"
#define TOPIC_RTG "remote_to_gateway"
#define QOS 1
#define TIMEOUT 10000L


/**
 * @brief 初始化MQTT
 * 
 * @return int 成功返回0，失败返回-1
 */
int app_mqtt_Init(void);

/**
 * @brief 关闭MQTT
 * 
 * @return int 成功返回0，失败返回-1
 */
int app_mqtt_Close(void);

/**
 * @brief 发布消息
 * 
 * @param json 发布的json数据
 * @return int 成功返回0，失败返回-1
 */
int app_mqtt_Publish(char *json);

/**
 * @brief 注册处理远程消息的回调函数
 * 
 * @param callback 回调函数 
 */
void app_mqtt_RegisterRecvCallback(int (*callback)(char *json));

#endif