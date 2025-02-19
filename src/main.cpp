#include <Arduino.h>
#include "motor.h"

// 设置传感器连接的GPIO引脚
const char SENSOR = 27;

Motor motor;

void setup() {
  pinMode(SENSOR, INPUT); // 设置传感器引脚为输入

  // 初始化电机
  motor.Init();

  Serial.begin(115200);
}

void loop() {
  static char flag_ball = 1;  // 进球标志
  static unsigned char sensorState = 0;

  if (flag_ball == 1) {
      if (digitalRead(SENSOR) == LOW) {
          flag_ball = 0;  // 触发进球行为
      } else {
          motor.Forward(77, 77);  // 电机直行
      }
  }

  if (flag_ball == 0) {
      flag_ball = -1;  // 自主部分结束
      // 若进球，暂停、右转、直行至安全区
     motor.TempStop(1000);
     motor.TurnRight(77, 77);

     motor.TempStop(500);
     motor.TempForward(3000, 77, 77); // 直行3秒
  }

  delay(100);  // 每100ms循环一次
}
