import pygame
import asyncio
import websockets

# ESP32 WebSocket 服务器的 IP 和端口
ESP32_IP = "192.168.155.151"
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
                    left = joystick.get_axis(1)   # 左轮
                    right = joystick.get_axis(3)  # 右轮
                    left_accele = joystick.get_axis(9) # 左轮加速键
                    right_accele = joystick.get_axis(10) # 右轮加速键

                    # 计算速度
                    if (left_accele == 1):
                        left_speed = int(abs(left) * 180)
                    else:
                        left_speed = int(abs(left) * 120)
                    if (right_accele == 1):
                        right_speed = int(abs(right) * 180)
                    else:
                        right_speed = int(abs(right) * 120)

                    # 方向判断
                    left_dir = 1 if left < -0.2 else -1 if left > 0.2 else 0
                    right_dir = 1 if right < -0.2 else -1 if right > 0.2 else 0

                    command = f"{left_dir},{left_speed},{right_dir},{right_speed}"

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
