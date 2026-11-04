# 曼波小狗 · Mambo Puppy (CH32V307 + RT-Thread 纯离线版)

桌面四足机器狗「曼波小狗」的 CH32V307 纯离线改造：主控由 STM32F103C8T6 替换为国产 **RISC-V** 芯片 **CH32V307RCT6**，固件由裸机移植到 **RT-Thread**，**纯离线**运行（无 WiFi、无云端 AI），通过 **ASRPRO 离线语音模块** 控制 4 个舵机与 OLED 表情。

## ✨ 特性

- **RISC-V 主控 144MHz**：CH32V307RCT6（青稞 V4F，144MHz / 256KB Flash / 64KB RAM，LQFP64）
- **纯离线**：无 WiFi、无蓝牙、无云端；语音识别与录音播放全部由 ASRPRO 离线模块完成
- **ASRPRO 语音**：单字符命令协议，支持语音唤醒（识别 + 录音播放）
- **4 × SG90 舵机**：TIM3 四路 50Hz PWM，前进 / 后退 / 转弯 / 跳舞 / 伸懒腰等全套步态
- **OLED 表情**：0.96" SSD1306（软件 I2C，地址 0x78），实时显示心情 / 体力值
- **RT-Thread**：cmd / action 双线程 + 邮箱，动作逻辑 100% 移植自原 manbo-robot-dog

## 🔌 硬件接线表

| 外设 | CH32V307RCT6 引脚 | 备注 |
|---|---|---|
| 舵机1 右前 | PA6 = TIM3_CH1 | 50Hz PWM |
| 舵机2 右后 | PA7 = TIM3_CH2 | 50Hz PWM |
| 舵机3 左前 | PB0 = TIM3_CH3 | 50Hz PWM |
| 舵机4 左后 | PB1 = TIM3_CH4 | 50Hz PWM |
| OLED SCL / SDA | PB8 / PB9 | 软件 I2C，地址 0x78 |
| 语音识别 ASR | PB10(TX) / PB11(RX) | USART3，9600，轮询收发 |
| LED | PC13 | 板载 |

> ⚠️ 舵机 5V 必须单独供电（330µF ×5 滤波），并与 CH32V307 共地。

## 🧩 STM32 → CH32V307 引脚映射表

已按 ST《DS5319》与 CH32V307 数据手册逐脚核实，完整版见 [docs/pin-mapping.md](docs/pin-mapping.md)。

| 功能 | 引脚名 | F103C8 LQFP48脚 | CH32V307 LQFP64脚 |
|---|---|---|---|
| 舵机1右前 | PA6 | 16 | 22 |
| 舵机2右后 | PA7 | 17 | 23 |
| 舵机3左前 | PB0 | 18 | 26 |
| 舵机4左后 | PB1 | 19 | 27 |
| OLED SCL | PB8 | 45 | 61 |
| OLED SDA | PB9 | 46 | 62 |
| ASR TX | PB10 | 21 | 29 |
| ASR RX | PB11 | 22 | 30 |
| LED | PC13 | 2 | 2 |
| SWDIO | PA13 | 34 | 46 |
| SWCLK | PA14 | 37 | 49 |
| NRST | — | 7 | 7 |
| BOOT0 | — | 44 | 60 |
| BOOT1 | PB2 | 20 | 28 |
| 晶振 | PC14/PC15 | 3/4 | 3/4 |
| 晶振 | PD0/PD1 | 5/6 | 5/6 |
| VDD | — | 24/36/48 | 32(VIO_1)/48/64(VIO_3) |
| VSS | — | 23/35/47 | 31/47/63 |
| VBAT/VDDA/VSSA | — | 1/9/8 | 1/13/12 |
| CH32V307独有 | VDD_4(19)→3.3V, VSS_4(18)→GND, PC0~PC12/PD2→悬空 | — | — |

## 🛠️ 转接板方案（LQFP48 → LQFP64）

原板焊接位为 STM32F103C8T6（LQFP48），新主控 CH32V307RCT6 为 LQFP64。为不改动原 PCB，采用 **13 × 13mm 双面转接板**：

- **底面**：LQFP48 焊盘（镜像布局），焊回原板主控位置
- **顶面**：CH32V307RCT6 + 0.1µF 去耦电容
- **走线原则**：全部引脚**同名互连**（PA6↔PA6、PB8↔PB8 ……）
- **焊接要点**：转接板**翻面焊接**，转接板引脚 1 对准原板引脚 1

| 器件 | LCSC 编号 |
|---|---|
| STM32F103C8T6（原板） | C8734 |
| CH32V307RCT6 | C2988083 |
| 100nF 去耦电容 | C14663 |

CH32V307 独有引脚处理：VDD_4(19)→3.3V，VSS_4(18)→GND，PC0~PC12 / PD2→悬空。
详见 [docs/adapter.md](docs/adapter.md)。

## 📁 目录结构

```
.
├── README.md                # 本文件
├── LICENSE                  # MIT 许可证
├── .gitignore
├── firmware/                # 固件源码（RT-Thread 应用层）
│   ├── dog_config.h         # 引脚 / 波特率 / 常量（唯一需要按板子修改的文件）
│   ├── dog.h                # 公共接口 + 全局状态
│   ├── bsp_dog.c            # 板级初始化 + LED
│   ├── drv_servo.c          # TIM3 四路舵机 PWM
│   ├── drv_movement.c       # 全部步态 / 动作
│   ├── drv_uart.c           # 语音识别串口（轮询收发）
│   ├── dog_mode.c           # 动作编排 + 命令分发 + 心情/体力（GBK 编码）
│   ├── app_dog.c            # RT-Thread 线程（cmd / action）
│   └── OLED.c / OLED.h / OLED_Data.c / OLED_Data.h   # 江协科技 SSD1306 驱动（已适配 ch32v30x）
└── docs/
    ├── pin-mapping.md       # STM32 → CH32V307 引脚映射表
    ├── adapter.md           # 转接板设计说明
    └── bom.md               # BOM 清单
```

## 🚀 快速开始

**前提**：用 RT-Thread Studio 基于官方 BSP「CH32V307V-R1」（路径 `bsp/wch/risc-v/ch32v307v-r1`）新建工程，或用 scons / Env 工具链。

1. 将 [firmware/](firmware/) 下所有 .c/.h 拷入工程的 `applications/`（或新建 `dog/` 目录并加入编译）。
2. 删除或覆盖工程自带的 `applications/main.c`（本工程 `app_dog.c` 已提供 `int main(void)`）。
3. 确认 `rtconfig.h`：
   - 开启 `RT_USING_MAILBOX`（动作线程用到邮箱）；
   - 关闭 `RT_USING_SERIAL`，或不启用 uart3 设备（固件直接操作 USART 外设，避免冲突）；
   - `rt_snprintf` 由内核提供，无需 newlib；未启用 libc 时也无需开 `RT_USING_LIBC`。
4. 编译烧录：MounRiver / RT-Thread Studio + WCH-Link。

### ⚠️ 必须核对 / 自测的 3 个点

1. **定时器时钟**：固件按 `DOG_TIM_CLK_HZ = 144MHz` 计算预分频。若你的板子主频不是 144MHz，
   改 `firmware/dog_config.h` 里的 `DOG_TIM_CLK_HZ`（舵机频率必须保持 50Hz，否则会抖、发热）。
2. **OLED 地址**：默认 0x78（`OLED.c` 内 `OLED_WriteCommand` / `OLED_WriteData` 里写死）。
   若你的屏是 0x7A / 0x3C，改那两处。
3. **中文编码**：`dog_mode.c` 必须保存为 GBK/GB2312（已按 GBK 写好），否则注释乱码。
   MounRiver：项目 → Properties → Resource → Text file encoding → GBK。

## 🗣️ 命令协议（单字符，语音）

| 字符 | 动作 | 字符 | 动作 |
|---|---|---|---|
| f / b | 前进 / 后退 | l / r | 左转 / 右转 |
| w / z | 前后摇 / 左右摇 | d | 跳舞 |
| 5 | 立正 | q | 起身 |
| s | 坐下擦脸 | j | 交替抬手 |
| y | 伸懒腰 | 1 | 抬头 |
| p | 趴下睡觉 | 2 | 卧下睡觉 |
| o | 打招呼 | n | 难受 |
| B / Y / X / W | 表白 / 元素周期表 / 校训 / 世界之光 | | |
| U | 语音唤醒 | K / T / Z | 查开心值 / 体力值 / 两者 |

## 🔗 参考链接

| 内容 | 链接 |
|---|---|
| 原工程（立创开源广场 oshwhub） | https://oshwhub.com/herobrine/stm32-mambo-robot-dog |
| 原固件源码（STM32F103） | https://github.com/RussellCooper-DJZ/manbo-robot-dog |
| 原版 B 站视频 | https://www.bilibili.com/video/BV1UmUfBNEwu |
| 外壳 3D 打印模型（MakerWorld） | https://makerworld.com.cn/zh/models/1937892-stm32man-bo-ji-qi-ren-wai-ke |

## 📄 声明与许可

本项目代码以 **MIT License** 发布（见 [LICENSE](LICENSE)，`Copyright (c) 2026 曼波小狗项目组`）。

第三方组件保留其各自权利：

- **OLED 驱动**（OLED.c / OLED_Data.c / OLED.h / OLED_Data.h）版权归**江协科技**，本项目仅做 CH32V307 适配；
- **动作 / 表情逻辑**版权归原 **manbo-robot-dog** 作者（https://github.com/RussellCooper-DJZ/manbo-robot-dog，MIT 许可）。

原硬件设计（PCB / 外壳）版权归原工程作者 herobrine（立创开源广场 oshwhub）。
