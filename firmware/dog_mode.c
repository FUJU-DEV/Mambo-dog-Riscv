#include "dog.h"

/* 本文件含中文注释，需以 GBK/GB2312 保存，避免乱码。
 * MounRiver: 项目右键 -> Properties -> Resource -> Text file encoding -> GBK */

int g_happiness = DOG_HAPPY_INIT;
int g_stamina   = DOG_STAMINA_INIT;
char g_cmd      = '0';
char g_prev_cmd = '0';


/* 简单 LCG 随机数，避免依赖 libc 的 rand() */
static rt_uint32_t g_rng = 0x12345678;
static int my_rand(void)
{
    g_rng = g_rng * 1664525u + 1013904223u;
    return (int)((g_rng >> 16) & 0x7fff);
}

/* 显示表情图。注意：江协驱动是“显存+Update”模型，必须调用 OLED_Update() 才刷新 */
static void face(const rt_uint8_t *bmp)
{
    OLED_ShowImage(0, 0, 128, 64, bmp);
    OLED_Update();
}

/* 在屏幕底部显示数值（用于 K/T/Z 查询） */
static void show_value(const char *tag, int value)
{
    OLED_ShowString(0, 48, (char *)tag, OLED_8X16);
    OLED_ShowNum(32, 48, (uint32_t)value, 3, OLED_8X16);
    OLED_Update();
}

/* ---------------- 各状态动作 ---------------- */
static void mode_forward(void)
{
    static rt_uint16_t ff = 0;
    face(BMP2); move_forward(); led_toggle();
    if (g_prev_cmd == 'f') { if (++ff >= 5) g_happiness -= 3; } else ff = 0;
    g_prev_cmd = g_cmd; g_stamina -= 5;
}

static void mode_behind(void)
{
    static rt_uint16_t bb = 0;
    face(BMP2); move_behind(); led_toggle();
    if (g_prev_cmd == 'b') { if (++bb >= 5) g_happiness -= 3; } else bb = 0;
    g_prev_cmd = g_cmd; g_stamina -= 5;
}

static void mode_left(void)
{
    static rt_uint16_t ll = 0;
    face(BMP3); move_left(); led_toggle();
    if (g_prev_cmd == 'l') { if (++ll >= 5) g_happiness -= 3; } else ll = 0;
    g_prev_cmd = g_cmd; g_stamina -= 5;
}

static void mode_right(void)
{
    static rt_uint16_t rr = 0;
    face(BMP4); move_right(); led_toggle();
    if (g_prev_cmd == 'r') { if (++rr >= 5) g_happiness -= 3; } else rr = 0;
    g_prev_cmd = g_cmd; g_stamina -= 5;
}

static void mode_swing_qianhou(void)
{
    face(BMP11); move_shake_qianhou(); led_toggle();
    g_prev_cmd = g_cmd; g_stamina -= 5; g_happiness += 5;
}

static void mode_swing_zuoyou(void)
{
    face(BMP11); move_shake_zuoyou(); led_toggle();
    g_prev_cmd = g_cmd; g_stamina -= 5; g_happiness += 5;
}

static void mode_dance(void)
{
    face(BMP5); move_dance(); led_toggle();
    g_prev_cmd = g_cmd; g_stamina -= 5; g_happiness += 5;
}

static void mode_stand(void)
{
    face(BMP1); move_stand(); led_toggle();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina -= 1;
}

static void mode_slowstand(void)
{
    face(BMP1); move_slow_stand(g_prev_cmd); led_toggle();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina -= 1;
}

static void mode_sit(void)
{
    face(BMP1); move_slow_stand(g_prev_cmd);
    face(BMP2); move_stretch(); face(BMP12); led_toggle();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina += 5; g_happiness += 2;
}

static void mode_hello(void)
{
    if (g_prev_cmd != '5') { face(BMP1); move_slow_stand(g_prev_cmd); }
    face(BMP12);
    move_hello();
    led_toggle();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina -= 2; g_happiness += 2;
}

static void mode_twohands(void)
{
    face(BMP1); move_stand(); move_two_hands(); led_toggle();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina -= 5; g_happiness += 2;
}

static void mode_lanyao(void)
{
    face(BMP1); move_slow_stand(g_prev_cmd);
    face(BMP9); move_lanyao(); face(BMP1); led_toggle();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina += 5; g_happiness += 1;
}

static void mode_headup(void)
{
    face(BMP1); move_slow_stand(g_prev_cmd);
    face(BMP10); move_head_up(); led_toggle();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina -= 5;
}

static void mode_sleeppa(void)
{
    if (g_prev_cmd != '5') { face(BMP1); move_slow_stand(g_prev_cmd); }
    if (my_rand() % 2) face(BMP6); else face(BMP8);
    move_sleep_p();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina = 100; g_happiness += 15;
}

static void mode_sleepwo(void)
{
    if (g_prev_cmd != '5') { face(BMP1); move_slow_stand(g_prev_cmd); }
    if (my_rand() % 2) face(BMP6); else face(BMP8);
    move_sleep_w();
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina = 100; g_happiness += 15;
}

static void mode_nanshou(void)
{
    face(BMP2); move_sleep_w();
    g_prev_cmd = g_cmd; g_cmd = '0';
}

static void mode_biaobai(void)
{
    face(BMP7); servo_set1(135);
    rt_thread_mdelay(8000);
    face(BMP11); servo_set3(45);
    rt_thread_mdelay(5000);
    servo_set1(90); servo_set3(90); face(BMP9);
    rt_thread_mdelay(5000);
    face(BMP7);
    move_shake_qianhou(); move_shake_qianhou();
    rt_thread_mdelay(5000);
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina -= 2; g_happiness += 5;
}

/* 元素周期表（Y）：按原版编排，每句 2s，最后 0.5s 收尾 */
static void mode_yuansu(void)
{
    face(BMP5); servo_set1(135);
    rt_thread_mdelay(2000);
    face(BMP10); servo_set3(45);
    rt_thread_mdelay(2000);
    servo_set2(45); servo_set4(135);
    rt_thread_mdelay(2000);
    face(BMP12);
    rt_thread_mdelay(2000);
    face(BMP11);
    move_shake_qianhou(); move_shake_qianhou(); move_shake_qianhou();
    rt_thread_mdelay(500);
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina -= 2; g_happiness += 2;
}

/* 校训（X）：按原版编排，每句 2s，最后 0.5s 收尾 */
static void mode_xiaoxun(void)
{
    face(BMP5); servo_set1(135);
    rt_thread_mdelay(2000);
    face(BMP10); servo_set3(45);
    rt_thread_mdelay(2000);
    servo_set2(45); servo_set4(135);
    rt_thread_mdelay(2000);
    face(BMP12);
    rt_thread_mdelay(2000);
    face(BMP11);
    move_shake_qianhou(); move_shake_qianhou();
    rt_thread_mdelay(500);
    g_prev_cmd = g_cmd; g_cmd = '0'; g_stamina -= 2;
}

static void mode_world(void)
{
    face(BMP5);
    servo_set2(135); servo_set4(45);
    rt_thread_mdelay(2000);
    g_prev_cmd = g_cmd; g_cmd = '0';
}

static void mode_xiaodai(void)
{
    face(BMP1);
    rt_thread_mdelay(1000);
    g_prev_cmd = g_cmd; g_cmd = '0';
}

/* 开心值（K）：表情 + 屏幕底部显示数值 */
static void mode_happiness(void)
{
    face(BMP1);
    if (g_happiness >= 60) {
        face(BMP9);
        move_shake_qianhou(); move_shake_qianhou();
    } else if (g_happiness <= 10) {
        face(BMP2);
        if (my_rand() % 2) move_sleep_p(); else move_sleep_w();
    }
    show_value("H:", g_happiness);
    rt_thread_mdelay(3000);
    g_prev_cmd = g_cmd; g_cmd = '0';
}

/* 体力值（T）：表情 + 屏幕底部显示数值 */
static void mode_stamina(void)
{
    face(BMP1);
    if (g_stamina >= 60) {
        face(BMP9);
        move_shake_qianhou(); move_shake_qianhou();
    } else if (g_stamina <= 10) {
        face(BMP2);
        if (my_rand() % 2) move_sleep_p(); else move_sleep_w();
    }
    show_value("S:", g_stamina);
    rt_thread_mdelay(4000);
    g_prev_cmd = g_cmd; g_cmd = '0';
}

/* 状态（Z）：同时显示开心值 + 体力值 */
static void mode_index(void)
{
    face(BMP1);
    show_value("H:", g_happiness);
    rt_thread_mdelay(1500);
    show_value("S:", g_stamina);
    rt_thread_mdelay(1500);
    g_prev_cmd = g_cmd; g_cmd = '0';
}

/* ---------------- 开机 ---------------- */
void dog_boot(void)
{
    face(BMP1);
}

/* ---------------- 命令分发 ---------------- */
void dog_dispatch(char cmd)
{
    /* 体力耗尽：只接受睡觉 */
    if (g_stamina <= 0) {
        if (cmd == '2') { g_cmd = cmd; mode_sleepwo(); }
        else if (cmd == 'p') { g_cmd = cmd; mode_sleeppa(); }
        return;
    }
    g_cmd = cmd;

    switch (cmd) {
        case 'f': mode_forward();     break;
        case 'b': mode_behind();      break;
        case 'l': mode_left();        break;
        case 'r': mode_right();       break;
        case 'w': mode_swing_qianhou(); break;
        case 'z': mode_swing_zuoyou();  break;
        case 'd': mode_dance();       break;
        case '5': mode_stand();       break;
        case 'q': mode_slowstand();   break;
        case 's': mode_sit();         break;
        case 'j': mode_twohands();    break;
        case 'y': mode_lanyao();      break;
        case '1': mode_headup();      break;
        case 'p': mode_sleeppa();     break;
        case '2': mode_sleepwo();     break;
        case 'o': mode_hello();       break;
        case 'n': mode_nanshou();     break;
        case 'B': mode_biaobai();     break;
        case 'Y': mode_yuansu();      break;
        case 'X': mode_xiaoxun();     break;
        case 'W': mode_world();       break;
        case 'U': mode_xiaodai();     break;
        case 'K': mode_happiness();   break;
        case 'T': mode_stamina();     break;
        case 'Z': mode_index();       break;
        default: break;
    }

    /* 数值夹紧 */
    if (g_happiness < 0)             g_happiness = 0;
    if (g_happiness > DOG_HAPPY_MAX) g_happiness = DOG_HAPPY_MAX;
    if (g_stamina < 0)               g_stamina = 0;
    if (g_stamina > DOG_STAMINA_MAX) g_stamina = DOG_STAMINA_MAX;
}
