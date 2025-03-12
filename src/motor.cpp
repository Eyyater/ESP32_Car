#include "motor.h"
#include <Arduino.h> 

// 引脚定义
const uint8_t LF = 22;
const uint8_t LB = 23;
const uint8_t RF = 15;
const uint8_t RB = 2;

// 构造函数：初始化电机状态
Motor::Motor() : left_dir(0), right_dir(0), 
	left_duty(0), right_duty(0) {}

// 初始化电机
void Motor::Init() {
    pinMode(LF, OUTPUT);
    pinMode(LB, OUTPUT);
    pinMode(RF, OUTPUT);
    pinMode(RB, OUTPUT);
    
    // 初始化PWM通道（无需重复设置占空比）
    ledcSetup(0, 10000, 8);  // LF
    ledcAttachPin(LF, 0);
    ledcSetup(1, 10000, 8);  // LB
    ledcAttachPin(LB, 1);
    ledcSetup(2, 10000, 8);  // RF
    ledcAttachPin(RF, 2);
    ledcSetup(3, 10000, 8);  // RB
    ledcAttachPin(RB, 3);
    
    Stop();  // 这会自动设置所有PWM为0
}

// 设置左电机
void Motor::SetLeftMotor(int dir, unsigned char duty) {
    left_dir = dir;
    left_duty = duty;
    
    if(dir == 1) { // 正转
        ledcWrite(0, duty);  // LF输出PWM
        ledcWrite(1, 0);     // LB置低
    } else if(dir == -1) {   // 反转
        ledcWrite(0, 0);     // LF置低
        ledcWrite(1, duty);  // LB输出PWM
    } else {                 // 停止
        ledcWrite(0, 0);
        ledcWrite(1, 0);
    }
}

// 设置右电机
void Motor::SetRightMotor(int dir, unsigned char duty) {
    right_dir = dir;
    right_duty = duty;
    
    if(dir == 1) {          // 正转
        ledcWrite(2, duty); // RF输出PWM
        ledcWrite(3, 0);    // RB置低
    } else if(dir == -1) {  // 反转
        ledcWrite(2, 0);    // RF置低
        ledcWrite(3, duty); // RB输出PWM
    } else {                // 停止
        ledcWrite(2, 0);
        ledcWrite(3, 0);
    }
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

void Motor::Brake() {
    // 左电机短路制动
    ledcWrite(0, 255);  // LF 高电平
    ledcWrite(1, 255);  // LB 高电平

    // 右电机短路制动
    ledcWrite(2, 255);  // RF 高电平
    ledcWrite(3, 255);  // RB 高电平

    delay(100);  // 短暂刹车后停止
    Stop();      // 彻底停止
}

// 直行指定毫秒数
void Motor::TempForward(int time,
	unsigned char leftDuty,
	unsigned char rightDuty
) {
    Forward(leftDuty, rightDuty);
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

// 遥控部分
void Motor::controlMotors(String message) {
    unsigned char right_speed, left_speed;
    int right_dir, left_dir;
    
    // 解析格式 "1,200,-1,180"
    sscanf(message.c_str(), "%d,%hhu,%d,%hhu",
    &left_dir, &left_speed, &right_dir, &right_speed);

    if (left_dir == 0 && right_dir == 0) {
        Brake();
    }
    else {
    // 控制左轮
    SetLeftMotor(left_dir, left_speed);
    // 控制右轮
    SetRightMotor(right_dir, right_speed);
    }
}