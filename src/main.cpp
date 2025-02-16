#include <Arduino.h>

// ESP32 发送信息给 STC89C52

void setup() {
    Serial.begin(115200); // 开始串口通信
    Serial2.begin(9600, SERIAL_8N1, 16, 17);  // 使用 Serial2 与 STC89C52 通信，假设使用GPIO 16和GPIO 17
    delay(1000);
  }
  
  void loop() {
    // 假设你要发送的信息
    String message = "Hello STC";
  
    // 发送信息
    Serial2.println(message);
    
    delay(1000); // 每秒发送一次
  }
  