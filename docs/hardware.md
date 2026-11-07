# 整机物料清单（A3 修复）

> 本仓库是**改造工程**：硬件 = 原版 oshwhub 板（曼波机器狗）+ 更换主控芯片。
> 原版板的 Gerber 在 [oshwhub 工程页](https://oshwhub.com/herobrine/stm32-mambo-robot-dog)，
> 可直接在立创下单打样。

## 一、PCB 板

| 物料 | 说明 | 数量 | 获取 |
|---|---|---|---|
| 原版 PCB | **曼波机器狗 OSHW 原板（5 舵机版 / 1S 3.7V / 1.3" OLED）** | 1 | [oshwhub 工程](https://oshwhub.com/herobrine/stm32-mambo-robot-dog) 打样/购买 |
| 主控替换 | **CH32V303CBT6**（LQFP-48，与 F103C8T6 同脚位直插） | 1 | 立创 C7494374（¥4.35） |
| 其他 PCB 器件 | 见 [bom.md](bom.md)（原板 17 项器件全部保留） | — | 立创 |

## 二、外购件（整机必需）

| 物料 | 规格 | 数量 | 参考价 | 获取 |
|---|---|---|---|---|
| 舵机 | SG90 / MG90S，9g，180°（原板 5 路：4 腿 + 尾巴可选） | 4 | ¥8-10/个 | 淘宝/立创 |
| 语音模块 | **天问 ASRPRO Core 板**（离线识别+录音） | 1 | ¥25-40 | 淘宝搜"天问ASRPRO" |
| 喇叭 | 8Ω 2W 或 8Ω 1W 小喇叭 | 1 | ¥3-5 | 淘宝 |
| 麦克风 | 驻极体 6mm（原板已含，型号 GMI6027P-2C30DB） | 1 | ¥1 | 立创 C233898 |
| OLED | 0.96" SSD1306，I2C，4Pin | 1 | ¥8-12 | 淘宝 |
| 电池 | **1S 锂电 3.7V**（原板 TP4057 单节充电 + ME6211；oshwhub 原板标注 1S 3.7V，可用 1×18650 或 1S 锂聚合物 1000-2000mAh） | 1 | ¥15-30 | 淘宝 |
| 充电 | TYPE-C 数据线（原板 USB-C 口） | 1 | ¥5 | 店配 |
| 调试器 | **WCH-LinkE**（USB 转 SWD 烧录/调试） | 1 | ¥20 | 淘宝/立创 |
| 外壳 | 3D 打印，MakerWorld 模型：[链接](https://makerworld.com.cn/zh/models/1937892-stm32man-bo-ji-qi-ren-wai-ke)（需浏览器打开，网站反爬） | 1 | 打印费 ¥20-60 | 3D 打印店 |

## 三、整机成本估算

- PCB 打样 + 器件：**约 ¥50**（含 CH32V303CBT6）
- 外购件（舵机/语音/喇叭/OLED/电池/调试器）：**约 ¥100**
- 外壳打印：**约 ¥30**
- **整机合计：约 ¥180**（不含运费）

## 四、接线总表

| 外设 | 引脚 | 说明 |
|---|---|---|
| 舵机1 右前 | PA10 = TIM1_CH3 | 50Hz PWM（原板 Servo1→CH3）|
| 舵机2 右后 | PA8 = TIM1_CH1 | 原板 Servo2→CH1 |
| 舵机3 左前 | PA11 = TIM1_CH4 | 原板 Servo3→CH4 |
| 舵机4 左后 | PA9 = TIM1_CH2 | 原板 Servo4→CH2 |
| ASRPRO | PB10(TX) / PB11(RX) | 9600 |
| OLED | PB8(SCL) / PB9(SDA) | 软件 I2C，0x78 |
| LED | PC13 | 低电平点亮 |
| SWD | PA13(SWDIO) / PA14(SWCLK) | WCH-Link 烧录 |
| 复位/启动 | NRST(P7) / BOOT0(P44) | BOOT0 拉高进 ISP |

> ⚠️ 舵机 5V 独立供电，必须与主控共地（原板已设计）。
