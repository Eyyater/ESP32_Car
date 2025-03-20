import pygame
import asyncio
import websockets

# ESP32 WebSocket 服务器的 IP 和端口
ESP32_IP = "192.168.240.152"
ESP32_PORT = 80
WS_URL = f"ws://{ESP32_IP}/ws"

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

async def handle_joystick():
    while True:
        try:
            async with websockets.connect(WS_URL) as ws:
                print("成功连接到 ESP32 WebSocket 服务器")

                while True:
                    for event in pygame.event.get():
                        if event.type == pygame.QUIT:
                            pygame.quit()
                            return

                    # 读取摇杆输入
                    straight = joystick.get_axis(1)   # 直行
                    turning = joystick.get_axis(0)  # 转弯
                    # left_accele = joystick.get_axis(9) # 左轮加速键
                    # right_accele = joystick.get_axis(10) # 右轮加速键

                    # 计算速度
                    # if (left_accele == 1):
                    #     left_speed = int(abs(left) * 180)
                    # else:
                    straight_speed = 100
                    # if (right_accele == 1):
                    #     right_speed = int(abs(right) * 180)
                    # else:
                    turning_speed = 50

                    # 方向判断
                    straight_dir = 1 if straight < -0.2 else -1 if straight > 0.2 else 0
                    turning_dir = 1 if turning < -0.2 else -1 if turning > 0.2 else 0

                    command = f"{straight_dir},{straight_speed},{turning_dir},{turning_speed}"

                    # 发送指令
                    await ws.send(command)
                    print(f"发送指令: {command}")

                    await asyncio.sleep(0.1)  # 限制发送频率，防止过载

        except websockets.exceptions.ConnectionClosedError:
            print("连接断开，尝试重连...")
            await asyncio.sleep(0.1)  # 等待 1 秒后重连
        except Exception as e:
            print(f"发生错误: {e}")
            await asyncio.sleep(0.1)

# 运行手柄控制协程
asyncio.run(handle_joystick())
