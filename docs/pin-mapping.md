# CH32V303CBT6 同脚位引脚表（LQFP-48）

**引脚号 = LQFP-48 芯片脚位 = STM32F103C8T6 的同一脚位，两者完全一致。**

原板主控为 STM32F103C8T6（LQFP-48），新主控 CH32V303CBT6 同为 LQFP-48 且**引脚编号逐一对应**，
因此原焊盘直接焊接即可，**无需改板、无需转接板，引脚 1 对引脚 1**。

## 外设引脚表

| 功能 | 引脚名 | LQFP48 脚 | 备注 |
|---|---|---|---|
| 舵机1右前 | PA10 | 31 | TIM1_CH3（原板通道顺序）|
| 舵机2右后 | PA8 | 29 | TIM1_CH1 |
| 舵机3左前 | PA11 | 32 | TIM1_CH4 |
| 舵机4左后 | PA9 | 30 | TIM1_CH2 |
| OLED SCL | PB8 | 45 | 软件 I2C |
| OLED SDA | PB9 | 46 | 软件 I2C |
| ASR TX | PB10 | 21 | USART3 |
| ASR RX | PB11 | 22 | USART3 |
| LED | PC13 | 2 | 低电平点亮 |
| SWDIO | PA13 | 34 | |
| SWCLK | PA14 | 37 | |
| NRST | — | 7 | |
| BOOT0 | — | 44 | |
| BOOT1 | PB2 | 20 | |
| 晶振 | PC14/PC15 | 3/4 | 原板 8MHz 晶振（必须保留：SDK 按 HSE×18=144MHz 配置，缺晶振会静默降到 8MHz） |
| 晶振 | PD0/PD1 | 5/6 | |
| 电源 | VDD×3 | 24/36/48 | 3.3V |
| 地 | VSS×3 | 23/35/47 | |
| 备份/模拟 | VBAT/VDDA/VSSA | 1/9/8 | |

## 引脚编号一一对应（与 STM32F103C8T6 完全相同）

PA6=16、PA7=17、PB0=18、PB1=19、PB2=20、PB10=21、PB11=22、VSS=23、VDD=24、PB12=25…PB15=28、
PA8=29…PA11=32、PA12=33、PA13=34、VSS=35、VDD=36、PA14=37、PA15=38、PB3=39…PB7=43、BOOT0=44、
PB8=45、PB9=46、VSS=47、VDD=48。

## 说明

- 上表同时适用于两种芯片：CH32V303CBT6 与 STM32F103C8T6 的 LQFP-48 脚位**逐脚相同**，
  不存在「CH32V307 独有电源脚 / PC0~PC12 悬空」这类差异（那是 LQFP-64 方案的历史问题，已作废）。
- 固件使用的引脚与外设与原版完全一致：TIM1 四路 PWM、USART3、GPIOB 软件 I2C、PC13 LED、SWD。
- 新固件使用内部时钟，晶振相关引脚（PC14/PC15、PD0/PD1）沿用原板布局即可，晶振可保留不焊。
- 焊接替换步骤见 [direct-replacement.md](direct-replacement.md)。
