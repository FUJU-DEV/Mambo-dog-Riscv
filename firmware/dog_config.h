#ifndef DOG_CONFIG_H
#define DOG_CONFIG_H

#include "ch32v30x.h"

/* =====================================================================
 * 曼波小狗 · CH32V307RCT6 + RT-Thread · 纯离线版
 * 引脚映射与原版 manbo-robot-dog 一致（以源码为准，非 README）
 * ===================================================================== */

/* ---------- 时钟 ----------
 * CH32V307 系统时钟 144MHz。TIM3 挂在 APB1：
 * APB1 = 72MHz，定时器时钟 = APB1 x2 = 144MHz。
 */
#define DOG_TIM_CLK_HZ       144000000UL

/* ---------- 舵机 PWM (TIM3, 4 通道, 50Hz) ----------
 * PA6 = CH1 右前腿    PA7 = CH2 右后腿
 * PB0 = CH3 左前腿    PB1 = CH4 左后腿
 */
#define SERVO_TIM            TIM3
#define SERVO_PSC            ((DOG_TIM_CLK_HZ / 1000000UL) - 1U)  /* 计数频率 1MHz */
#define SERVO_ARR            19999U                               /* 20ms = 50Hz */

/* ---------- 串口 ----------
 * USART3 = 语音识别(ASRPRO)  PB10(TX) / PB11(RX)  9600
 */
#define UART_ASR              USART3
#define UART_ASR_BAUD         9600U

/* ---------- OLED (0.96" SSD1306, 软件 I2C, 江协科技驱动) ---------- */
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
