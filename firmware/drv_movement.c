#include "dog.h"

/* 动作库：角度序列 100% 来自原版 manbo-robot-dog 的 Movement.c，
 * 仅把 Delay_ms 换成 rt_thread_mdelay。 */

static rt_int32_t g_move_delay = DOG_MOVE_DELAY_MS;

static void d(rt_uint32_t ms) { rt_thread_mdelay(ms); }

void move_stand(void)
{
    servo_set1(90); servo_set2(90); servo_set3(90); servo_set4(90);
    d(500);
}

void move_forward(void)
{
    servo_set1(135); servo_set4(45);  d(g_move_delay);
    servo_set2(45);  servo_set3(135); d(g_move_delay);
    servo_set1(90);  servo_set4(90);  d(g_move_delay);
    servo_set2(90);  servo_set3(90);  d(g_move_delay);
    servo_set2(135); servo_set3(45);  d(g_move_delay);
    servo_set1(45);  servo_set4(135); d(g_move_delay);
    servo_set2(90);  servo_set3(90);  d(g_move_delay);
    servo_set1(90);  servo_set4(90);  d(g_move_delay);
}

void move_behind(void)
{
    servo_set1(45);  servo_set4(135); d(g_move_delay);
    servo_set2(135); servo_set3(45);  d(g_move_delay);
    servo_set1(90);  servo_set4(90);  d(g_move_delay);
    servo_set2(90);  servo_set3(90);  d(g_move_delay);
    servo_set2(45);  servo_set3(135); d(g_move_delay);
    servo_set1(135); servo_set4(45);  d(g_move_delay);
    servo_set2(90);  servo_set3(90);  d(g_move_delay);
    servo_set1(90);  servo_set4(90);  d(g_move_delay);
}

void move_right(void)
{
    servo_set1(45);  servo_set4(45);  d(g_move_delay);
    servo_set2(135); servo_set3(135); d(g_move_delay);
    servo_set1(90);  servo_set4(90);  d(g_move_delay);
    servo_set2(90);  servo_set3(90);  d(g_move_delay);
}

void move_right_hw(void)
{
    servo_set1(45);  servo_set4(45);  d(g_move_delay);
    servo_set2(135); servo_set3(135); d(g_move_delay);
    servo_set1(90);  servo_set4(90);
    servo_set2(90);  servo_set3(90);  d(g_move_delay);
}

void move_left(void)
{
    servo_set2(135); servo_set3(135); d(g_move_delay);
    servo_set1(45);  servo_set4(45);  d(g_move_delay);
    servo_set2(90);  servo_set3(90);  d(g_move_delay);
    servo_set1(90);  servo_set4(90);  d(g_move_delay);
}

void move_left_hw(void)
{
    servo_set2(135); servo_set3(135); d(g_move_delay);
    servo_set1(45);  servo_set4(45);  d(g_move_delay);
    servo_set2(90);  servo_set3(90);
    servo_set1(90);  servo_set4(90);  d(g_move_delay);
}

void move_hello(void)
{
    rt_int32_t i;
    for (i = 0; i < 20; i++) { servo_set1(90 - i); servo_set2(90 + i); servo_set4(90 - i); d(4); }
    for (i = 0; i < 60; i++) { servo_set2(110 + i); servo_set4(70 - i); d(4); }
    for (i = 0; i < 60; i++) { servo_set1(70 + i); d(4); }
    d(50);
    servo_set1(180); d(500);
    servo_set1(130); d(500);
    servo_set1(180); d(500);
    servo_set1(130); d(500);
    servo_set1(70);  d(500);
}

void move_shake_qianhou(void)
{
    servo_set2(135); servo_set1(135); servo_set3(45);  servo_set4(45);  d(150);
    servo_set1(90);  servo_set2(90);  servo_set3(90);  servo_set4(90);  d(150);
    servo_set2(45);  servo_set1(45);  servo_set3(135); servo_set4(135); d(150);
    servo_set1(90);  servo_set2(90);  servo_set3(90);  servo_set4(90);  d(150);
}

void move_shake_zuoyou(void)
{
    servo_set1(135); servo_set2(135); d(g_move_delay);
    servo_set3(135); servo_set4(135); d(g_move_delay);
    servo_set1(90);  servo_set2(90);  d(g_move_delay);
    servo_set3(90);  servo_set4(90);  d(g_move_delay);
    servo_set3(45);  servo_set4(45);  d(g_move_delay);
    servo_set1(45);  servo_set2(45);  d(g_move_delay);
    servo_set3(90);  servo_set4(90);  d(g_move_delay);
    servo_set1(90);  servo_set2(90);  d(g_move_delay);
}

void move_dance(void)
{
    servo_set1(135); servo_set2(135); d(g_move_delay);
    servo_set3(135); servo_set4(135); d(g_move_delay);
    servo_set1(90);  servo_set2(90);  d(g_move_delay);
    servo_set3(90);  servo_set4(90);  d(g_move_delay);
    servo_set2(135); servo_set1(135); servo_set3(45); servo_set4(45); d(150);
    servo_set1(90);  servo_set2(90);  servo_set3(90); servo_set4(90); d(150);
    servo_set3(45);  servo_set4(45);  d(g_move_delay);
    servo_set1(45);  servo_set2(45);  d(g_move_delay);
    servo_set3(90);  servo_set4(90);  d(g_move_delay);
    servo_set1(90);  servo_set2(90);  d(g_move_delay);
    servo_set2(45);  servo_set1(45);  servo_set3(135); servo_set4(135); d(150);
    servo_set1(90);  servo_set2(90);  servo_set3(90);  servo_set4(90);  d(150);
}

void move_head_up(void)
{
    rt_int32_t i;
    for (i = 0; i < 20; i++) { servo_set1(90 - i); servo_set3(90 + i); d(10); }
    for (i = 0; i < 65; i++) { servo_set2(90 + i); servo_set4(90 - i); d(10); }
}

void move_slow_stand(char prev)
{
    rt_uint8_t a1, a2, a3, a4;
    if (prev == '0') return;
    a1 = servo_get1(); a2 = servo_get2(); a3 = servo_get3(); a4 = servo_get4();
    while (a1 != 90 || a2 != 90 || a3 != 90 || a4 != 90) {
        if (a1 > 90) servo_set1(--a1); else if (a1 < 90) servo_set1(++a1);
        if (a2 > 90) servo_set2(--a2); else if (a2 < 90) servo_set2(++a2);
        if (a3 > 90) servo_set3(--a3); else if (a3 < 90) servo_set3(++a3);
        if (a4 > 90) servo_set4(--a4); else if (a4 < 90) servo_set4(++a4);
        d(10);
    }
}

void move_stretch(void)
{
    rt_int32_t i;
    for (i = 0; i < 65; i++) { servo_set2(90 + i); servo_set4(90 - i); d(5); }
    for (i = 0; i < 20; i++) { servo_set1(90 - i); servo_set3(90 + i); d(5); }
    d(1000);
    for (i = 0; i < 60; i++) { servo_set1(70 + i); d(4); }
    d(1000);
    servo_set1(180); d(500);
    servo_set1(130); d(500);
    servo_set1(180); d(500);
    servo_set1(130); d(500);
    servo_set1(70);  d(5);
}

void move_two_hands(void)
{
    servo_set3(20);  servo_set2(20);  d(200);
    servo_set3(90);  servo_set2(90);  d(200);
    servo_set1(160); servo_set4(160); d(200);
    servo_set1(90);  servo_set4(90);  d(200);
}

void move_lanyao(void)
{
    rt_int32_t i;
    for (i = 0; i < 75; i++) { servo_set1(90 + i); servo_set3(90 - i); servo_set2(90 + i / 2); servo_set4(90 - i / 2); d(5); }
    d(g_move_delay * 50);
    for (i = 0; i < 75; i++) { servo_set1(165 - i); servo_set3(15 + i); servo_set2(127 - i / 2); servo_set4(53 + i / 2); d(5); }
    d(g_move_delay);
}

void move_sleep_p(void)
{
    rt_int32_t i;
    for (i = 0; i < 75; i++) { servo_set1(90 + i); servo_set3(90 - i); d(10); }
    for (i = 0; i < 75; i++) { servo_set4(90 + i); servo_set2(90 - i); d(10); }
}

void move_sleep_w(void)
{
    rt_int32_t i;
    for (i = 0; i < 75; i++) { servo_set3(90 + i); servo_set1(90 - i); d(10); }
    for (i = 0; i < 75; i++) { servo_set2(90 + i); servo_set4(90 - i); d(10); }
}
