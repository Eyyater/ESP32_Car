#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "motor.h"

// WiFi 配置
const char* ssid = "HONOR 60 Pro";  
const char* password = "cccccccc";  

// 创建 WebSocket 服务器
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// 设置传感器连接的GPIO引脚
const char SENSOR = 26;
char flag_ball = 1;

Motor motor;

void onWebSocketEvent(AsyncWebSocket *server,
    AsyncWebSocketClient *client,
    AwsEventType type, void *arg,
    uint8_t *data,
    size_t len
) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("客户端 %u 连接\n", client->id());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("客户端 %u 断开连接\n", client->id());
    } else if (type == WS_EVT_DATA) {
        String message = "";
        for (size_t i = 0; i < len; i++) {
            message += (char)data[i];
        }
        Serial.printf("收到消息: %s\n", message.c_str());

        if (flag_ball == 255)
            motor.controlMotors(message);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(SENSOR, INPUT_PULLUP);

    // 初始化电机
    motor.Init();

    // 启动 WiFi 连接（但不等待）
    WiFi.begin(ssid, password);
    Serial.println("WiFi 连接中...");
}


void loop() {
    static bool wifiConnected = false;  // 记录 WiFi 状态

    // 只在 WiFi 状态发生变化时打印信息
    if (WiFi.status() == WL_CONNECTED && !wifiConnected) {
        wifiConnected = true;
        Serial.print("\nWiFi 连接成功，IP 地址: ");
        Serial.println(WiFi.localIP());

        // WiFi 连接成功后启动 WebSocket 服务器
        ws.onEvent(onWebSocketEvent);
        WiFi.setSleep(false);  // 关闭 WiFi 省电模式
        server.addHandler(&ws);
        server.begin();
        Serial.println("WebSocket 服务器已启动");
    } 
    else if (WiFi.status() != WL_CONNECTED && wifiConnected) {
        wifiConnected = false;
        Serial.println("WiFi 断开，正在重新连接...");
        WiFi.disconnect();
        WiFi.begin(ssid, password);
    }

    // 执行小车控制逻辑（不受 WiFi 影响）
    static unsigned char sensorState = 0;  

    if (flag_ball == 1) {
        if (digitalRead(SENSOR) == LOW) {
            flag_ball = 0;
        } else {
            motor.Forward(120, 120);
        }
        delay(100);
    }

    if (flag_ball == 0) {
        motor.TempStop(1000);
        motor.TurnRight(100, 100);
        motor.TempStop(500);
        motor.TempForward(1800, 120, 120);
        flag_ball = -1;
    }

    // Serial.printf("flag_ball = %hhu\n", flag_ball);
}
