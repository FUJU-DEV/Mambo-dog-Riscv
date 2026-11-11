#ifndef DOG_H
#define DOG_H

#include <rtthread.h>
#include "dog_config.h"

/* 动作时序依赖 1ms tick：RT_TICK_PER_SECOND 必须是 1000 */
#if RT_TICK_PER_SECOND != 1000
#error "RT_TICK_PER_SECOND 必须为 1000（动作时序/步态精度依赖 1ms tick）"
#endif

/* ---- 全局状态 ---- */
extern int         g_happiness;
extern int         g_stamina;
extern char        g_cmd;          /* 当前命令 */
extern char        g_prev_cmd;     /* 上一次命令 */

/* ---- 板级初始化 ---- */
void bsp_dog_init(void);
void led_toggle(void);

/* ---- 舵机 ---- */
void servo_init(void);
void servo_set1(float angle);
void servo_set2(float angle);
void servo_set3(float angle);
void servo_set4(float angle);
rt_uint8_t servo_get1(void);
rt_uint8_t servo_get2(void);
rt_uint8_t servo_get3(void);
rt_uint8_t servo_get4(void);

/* ---- 动作库 ---- */
void move_stand(void);
void move_forward(void);
void move_behind(void);
void move_right(void);
void move_right_hw(void);
void move_left(void);
void move_left_hw(void);
void move_hello(void);
void move_shake_qianhou(void);
void move_shake_zuoyou(void);
void move_dance(void);
void move_head_up(void);
void move_slow_stand(char prev);
void move_stretch(void);
void move_two_hands(void);
void move_lanyao(void);
void move_sleep_p(void);
void move_sleep_w(void);

/* ---- 串口（轮询收发，不用中断） ---- */
void uart_dog_init(void);
rt_bool_t uart_cmd_poll(char *ch);  /* 轮询语音识别(USART3) */

/* ---- 模式编排 ---- */
void dog_boot(void);
void dog_dispatch(char cmd);

/* ---- OLED（江协科技驱动接口，见 OLED.h） ---- */
#include "OLED.h"

#endif /* DOG_H */
