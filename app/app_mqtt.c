#include "app_mqtt.h"
#include <MQTTClient.h>
#include "log/log.h"
#include <string.h>

// mqtt客户端句柄
static MQTTClient client;
// mqtt默认配置
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
// 初始化发布消息默认配置
static MQTTClient_message pubmsg = MQTTClient_message_initializer;
// 处理远程消息的回调函数
static int (*recv_callback)(char *json) = NULL;

// 发布消息完成的回调函数
void delivered(void *context, MQTTClient_deliveryToken dt)
{
    log_debug("消息已发布");
}

// 接收消息的回调函数
// 成功处理接收消息返回1，失败返回0
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int result = 0;
    if (recv_callback != NULL)
    {
        result = recv_callback(message->payload) == 0 ? 1 : 0;
    }
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return result;
}

// 断线处理函数
void connlost(void *context, char *cause)
{
    log_debug("连接断开,原因:%s", cause);
}

int app_mqtt_Init(void)
{
    // 创建mqtt客户端
    if (MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_error("创建mqtt客户端失败");
        return -1;
    }
    // 设置mqtt客户端回调函数
    if (MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered) != MQTTCLIENT_SUCCESS)
    {
        log_error("设置MQTT客户端的回调函数失败");
        // 销毁mqtt客户端句柄
        MQTTClient_destroy(&client);
        return -1;
    }
    // 连接mqtt服务器
    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS)
    {
        log_error("连接mqtt服务器失败");
        // 销毁mqtt客户端句柄
        MQTTClient_destroy(&client);
        return -1;
    }
    // 订阅主题
    if (MQTTClient_subscribe(client, TOPIC_RTG, QOS) != MQTTCLIENT_SUCCESS)
    {
        log_error("订阅主题失败");
        // 销毁mqtt客户端句柄
        MQTTClient_destroy(&client);
        // 断开服务器连接
        MQTTClient_disconnect(client, TIMEOUT);
        return -1;
    }
    log_debug("mqtt初始化成功");
    return 0;
}

int app_mqtt_Close(void)
{
    // 取消订阅
    MQTTClient_unsubscribe(client, TOPIC_RTG);
    // 断开服务器连接
    MQTTClient_disconnect(client, TIMEOUT);
    // 销毁mqtt客户端句柄
    MQTTClient_destroy(&client);
    return 0;
}

int app_mqtt_Publish(char *json)
{
    pubmsg.payload = json;
    pubmsg.payloadlen = (int)strlen(json);
    pubmsg.qos = QOS;
    if (MQTTClient_publishMessage(client, TOPIC_GTR, &pubmsg, NULL) != MQTTCLIENT_SUCCESS)
    {
        log_error("发布消息失败");
        return -1;
    }
    return 0;
}

void app_mqtt_RegisterRecvCallback(int (*callback)(char *json))
{
    recv_callback = callback;
}
