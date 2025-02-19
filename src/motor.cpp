#include "motor.h"
#include <Arduino.h> 

// 引脚定义
const uint8_t LF = 35;
const uint8_t LB = 36;
const uint8_t RF = 37;
const uint8_t RB = 38;

// 构造函数：初始化电机状态
Motor::Motor() : left_dir(0), right_dir(0), 
	left_duty(0), right_duty(0) {}

// 初始化电机
void Motor::Init() {
    pinMode(LF, OUTPUT);
    pinMode(LB, OUTPUT);
    pinMode(RF, OUTPUT);
    pinMode(RB, OUTPUT);
    Stop();  // 初始化时停止电机
    
    SetupPWM(0, LF, left_duty);
    SetupPWM(1, LB, left_duty);
    SetupPWM(2, RF, right_duty);
    SetupPWM(3, RB, right_duty);
}

// 设置 PWM
void Motor::SetupPWM(int channel, int pin, unsigned char duty) {
    ledcSetup(channel, 5000, 8);   // 设置 PWM 频率和分辨率
    ledcAttachPin(pin, channel);    // 绑定引脚到 PWM 通道
    ledcWrite(channel, duty);       // 设置占空比
}

// 设置左电机
void Motor::SetLeftMotor(char dir, unsigned char duty) {
    left_dir = dir;
    left_duty = duty;
}

// 设置右电机
void Motor::SetRightMotor(char dir, unsigned char duty) {
    right_dir = dir;
    right_duty = duty;
}

// 前进
void Motor::Forward(
	unsigned char leftDuty,
	unsigned char rightDuty
) {
    SetLeftMotor(1, leftDuty);
    SetRightMotor(1, rightDuty);
}

// 后退
void Motor::Backward(
	unsigned char leftDuty,
	unsigned char rightDuty
) {
    SetLeftMotor(-1, leftDuty);
    SetRightMotor(-1, rightDuty);
}

// 停止电机
void Motor::Stop() {
    SetLeftMotor(0, 0);
    SetRightMotor(0, 0);
}

// 直行指定毫秒数
void Motor::TempForward(int time,
	unsigned char leftDuty,
	unsigned char rightDuty
) {
    SetLeftMotor(1, leftDuty);
    SetRightMotor(1, rightDuty);
    delay(time);  // 延时，模拟运行时间
    Stop();  // 停止
}

// 暂停指定毫秒数
void Motor::TempStop(int time) {
    Stop();
    delay(time);  // 延时
}

// 左转
void Motor::TurnLeft(
	unsigned char leftDuty,
	unsigned char rightDuty
) {
    SetLeftMotor(-1, leftDuty);
    SetRightMotor(1, rightDuty);
    delay(200);  // 延时，模拟转向时间
    Stop();
}

// 右转
void Motor::TurnRight(
	unsigned char leftDuty,
	unsigned char rightDuty
) {
    SetLeftMotor(1, leftDuty);
    SetRightMotor(-1, rightDuty);
    delay(200);  // 延时，模拟转向时间
    Stop();
}
