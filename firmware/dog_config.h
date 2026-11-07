#ifndef DOG_CONFIG_H
#define DOG_CONFIG_H

#include "ch32v30x.h"

/* =====================================================================
 * 曼波小狗 · CH32V303CBT6 + RT-Thread · 纯离线版
 * 硬件基准：oshwhub 原板（5 舵机版 / 1S 3.7V / 1.3" OLED 可选 0.96"）
 * 舵机 = TIM1_CH1~CH4（PA8/PA9/PA10/PA11），按原板 Servo.c 的通道顺序：
 *   Servo1(右前)→CH3(PA10)  Servo2(右后)→CH1(PA8)
 *   Servo3(左前)→CH4(PA11)  Servo4(左后)→CH2(PA9)
 *   （第 5 路尾巴 Servo5→TIM2_CH1(PA15)，本工程已裁剪）
 * 时钟：CH32V303 必须用 8MHz 外部晶振（HSE×18=144MHz，SDK 的
 *   SystemInit 只按 HSE 配置；晶振缺失会静默停在内部 8MHz）。
 * ===================================================================== */

/* ---------- 时钟 ----------
 * CH32V303CBT6 系统时钟 144MHz（HSE 8MHz ×18）。
 * TIM1 挂在 APB2：APB2 = 144MHz，定时器时钟 = 144MHz。
 */
#define DOG_TIM_CLK_HZ       144000000UL

/* ---------- 舵机 PWM (TIM1, 4 通道, 50Hz) ----------
 * PA8 = CH1（右后）  PA9  = CH2（左后）
 * PA10 = CH3（右前） PA11 = CH4（左前）
 */
#define SERVO_TIM            TIM1
#define SERVO_PSC            ((DOG_TIM_CLK_HZ / 1000000UL) - 1U)  /* 计数频率 1MHz */
#define SERVO_ARR            19999U                               /* 20ms = 50Hz */

/* ---------- 串口 ----------
 * USART3 = 语音识别(ASRPRO)  PB10(TX) / PB11(RX)
 * 注意：原板随板工程波特率为 115200；ASRPRO 模块可在天问Block 里改，
 *       本工程默认 9600（与固件一致即可）。
 */
#define UART_ASR              USART3
#define UART_ASR_BAUD         9600U

/* ---------- OLED (0.96"/1.3" SSD1306, 软件 I2C, 江协科技驱动) ---------- */
#define OLED_PORT             GPIOB
#define OLED_SCL              GPIO_Pin_8
#define OLED_SDA              GPIO_Pin_9

/* ---------- LED ---------- */
#define LED_PORT              GPIOC
#define LED_PIN               GPIO_Pin_13

/* ---------- 动作参数 ---------- */
#define DOG_MOVE_DELAY_MS     150     /* 步态间隔，原版 movedelay */

/* ---------- 宠物状态 ---------- */
#define DOG_HAPPY_INIT        200
#define DOG_HAPPY_MAX         200
#define DOG_STAMINA_INIT      500
#define DOG_STAMINA_MAX       500

#endif /* DOG_CONFIG_H */
