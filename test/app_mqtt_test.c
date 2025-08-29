#include "app_mqtt.h"
#include "log/log.h"
#include <unistd.h>

int receive_callback(char *json)
{
    log_debug("处理接收到的远程消息：%s", json);

    return 0;
}
int main(int argc, char const *argv[])
{
    // 初始化mqtt
    app_mqtt_Init();
    // 注册处理接收到的消息的回调函数
    app_mqtt_RegisterRecvCallback(receive_callback);
    // 发布一个消息
    app_mqtt_Publish("{\"conn_type\":1,\"id\":\"5859\",\"msg\":\"61626364\"}");
    // 休眠1s，处理消息
    sleep(100);
    // 关闭mqtt
    app_mqtt_Close();

    return 0;
}
