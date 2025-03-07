import pygame
# import websocket

# 初始化 Pygame 和手柄
pygame.init()
pygame.joystick.init()

# 检测是否有手柄连接
if pygame.joystick.get_count() == 0:
    print("未检测到手柄，请插入后重试！")
    pygame.quit()
    exit()

# 获取手柄对象
joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"检测到手柄: {joystick.get_name()}")

# 监听手柄输入
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        # 获取左摇杆 Y 轴（左轮前进/后退）
        left = joystick.get_axis(1)  # -1 向上（前进），1 向下（后退）
        # 获取右摇杆 Y 轴（右轮前进/后退）
        right= joystick.get_axis(3)  # -1 向上（前进），1 向下（后退）

        # 设定运动速度
        left_speed = int(abs(left) * 255)  # 转换为 0-255
        right_speed = int(abs(right) * 255)
        

        # 生成运动指令
        if left < -0.2:  # 左前进
            left_dir = 1;
        elif left > 0.2:  # 左后退
            left_dir = -1;
        else:
            left_dir = 0
        
        if right < -0.2:  # 右前进
            right_dir = 1;
        elif right > 0.2:  # 右后退
            right_dir = -1;
        else:
            right_dir = 0
        
        command = f"{left_dir},{left_speed},{right_dir},{right_speed}"

        # 发送指令到 ESP32
        # ws.send(command)
        print(f"发送指令: {command}")

pygame.quit()
# ws.close()