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
        // Serial.printf("收到消息: %s\n", message.c_str());

        // if (flag_ball == -1)
        //     motor.controlMotors(message);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(SENSOR, INPUT_PULLUP);
  
    // 初始化电机
    motor.Init();
  
    // // 连接 WiFi
    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    //     Serial.print(".");
    // }
    // Serial.println("\nWiFi 连接成功！");
    // Serial.print("ESP32 IP 地址: ");
    // Serial.println(WiFi.localIP());
  
    // // 配置 WebSocket 服务器
    // ws.onEvent(onWebSocketEvent);
    // WiFi.setSleep(false);  // 关闭WiFi省电模式，减少断连
    // server.addHandler(&ws);
  
    // // 启动 Web 服务器
    // server.begin();
    // Serial.println("WebSocket 服务器已启动");
}

void loop() {
  static unsigned char sensorState = 0;
  static char flag_ball = 1;  // 进球标志

  if (flag_ball == 1) {
      if (digitalRead(SENSOR) == LOW) {
          flag_ball = 0;  // 触发进球行为
      } 
      else {
          motor.Forward(77, 77);  // 电机直行
      }
      delay(100);  // 每100ms循环一次
  }

  if (flag_ball == 0) {
      // 若进球，暂停、右转、直行至安全区
     motor.TempStop(1000);
     motor.TurnRight(77, 77);

     motor.TempStop(500);
     motor.TempForward(3000, 77, 77); // 直行3秒
     
     flag_ball = -1;  // 自主部分结束
  }
}
