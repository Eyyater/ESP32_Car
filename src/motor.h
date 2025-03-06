#ifndef MOTOR_H
#define MOTOR_H

class Motor {
public:
    Motor();  // 构造函数，初始化电机状态
    void Init();  // 初始化电机
    void Test();  //测试函数
    void SetupPWM(int channel, int pin, unsigned char duty);  // 设置 PWM
    void SetLeftMotor(int dir, unsigned char duty);  // 设置左电机
    void SetRightMotor(int dir, unsigned char duty);  // 设置右电机
    void Forward(unsigned char leftDuty, unsigned char rightDuty);  // 电机前进
    void Backward(unsigned char leftDuty, unsigned char rightDuty);  // 电机后退
    void Stop();  // 停止电机
    void TempForward(int time, unsigned char leftDuty, unsigned char rightDuty);  // 直行一段时间
    void TempStop(int time);  // 暂停一段时间
    void TurnLeft(unsigned char leftDuty, unsigned char rightDuty);  // 左转
    void TurnRight(unsigned char leftDuty, unsigned char rightDuty);  // 右转

private:
    char left_dir, right_dir;  // 电机方向
    unsigned char left_duty, right_duty;  // 电机转速
};

#endif