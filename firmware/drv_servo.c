#include "dog.h"

/* 舵机 PWM：TIM1 CH1~CH4（PA8/PA9/PA10/PA11），按原板 Servo.c 的通道顺序
 *   servo_set1 → TIM1_CH3 (PA10) 右前
 *   servo_set2 → TIM1_CH1 (PA8)  右后
 *   servo_set3 → TIM1_CH4 (PA11) 左前
 *   servo_set4 → TIM1_CH2 (PA9)  左后
 * TIM1 是高级定时器，必须开主输出（MOE），否则 CH1~4 无波形。
 */

#define SERVO_MIN_CCR  500U   /* 0.5ms -> 0 度 */
#define SERVO_MAX_CCR  2500U  /* 2.5ms -> 180 度 */

static rt_uint16_t angle_to_ccr(float angle)
{
    if (angle <= 0.0f)   return SERVO_MIN_CCR;
    if (angle >= 180.0f) return SERVO_MAX_CCR;
    return (rt_uint16_t)(angle * (float)(SERVO_MAX_CCR - SERVO_MIN_CCR) / 180.0f + SERVO_MIN_CCR);
}

/* CCR -> 角度，带上下限夹紧（否则未初始化的通道 0 会回绕成 110°） */
static rt_uint8_t ccr_to_angle(rt_uint16_t ccr)
{
    rt_int32_t v;
    if (ccr <= SERVO_MIN_CCR) return 0;
    if (ccr >= SERVO_MAX_CCR) return 180;
    v = ((rt_int32_t)ccr - (rt_int32_t)SERVO_MIN_CCR) * 180 / (rt_int32_t)(SERVO_MAX_CCR - SERVO_MIN_CCR);
    if (v < 0) v = 0;
    if (v > 180) v = 180;
    return (rt_uint8_t)v;
}

void servo_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef g;
    g.GPIO_Mode  = GPIO_Mode_AF_PP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;  /* TIM1 CH1~CH4 */
    GPIO_Init(GPIOA, &g);

    TIM_TimeBaseInitTypeDef tb;
    tb.TIM_Prescaler         = (rt_uint16_t)SERVO_PSC;   /* 143 -> 1MHz */
    tb.TIM_CounterMode       = TIM_CounterMode_Up;
    tb.TIM_Period            = SERVO_ARR;                /* 19999 -> 50Hz */
    tb.TIM_ClockDivision     = TIM_CKD_DIV1;
    tb.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(SERVO_TIM, &tb);

    TIM_OCInitTypeDef oc;
    oc.TIM_OCMode        = TIM_OCMode_PWM1;
    oc.TIM_OutputState   = TIM_OutputState_Enable;
    oc.TIM_OutputNState  = TIM_OutputNState_Disable;
    oc.TIM_Pulse         = 0;
    oc.TIM_OCPolarity    = TIM_OCPolarity_High;
    oc.TIM_OCNPolarity   = TIM_OCPolarity_High;
    oc.TIM_OCIdleState   = TIM_OCIdleState_Reset;
    oc.TIM_OCNIdleState  = TIM_OCNIdleState_Reset;
    TIM_OC1Init(SERVO_TIM, &oc);
    TIM_OC2Init(SERVO_TIM, &oc);
    TIM_OC3Init(SERVO_TIM, &oc);
    TIM_OC4Init(SERVO_TIM, &oc);

    TIM_CtrlPWMOutputs(SERVO_TIM, ENABLE);   /* 高级定时器必须开主输出 */
    TIM_Cmd(SERVO_TIM, ENABLE);
}

/* 通道顺序按原板：1→CH3(PA10)、2→CH1(PA8)、3→CH4(PA11)、4→CH2(PA9) */
void servo_set1(float a) { TIM_SetCompare3(SERVO_TIM, angle_to_ccr(a)); }
void servo_set2(float a) { TIM_SetCompare1(SERVO_TIM, angle_to_ccr(a)); }
void servo_set3(float a) { TIM_SetCompare4(SERVO_TIM, angle_to_ccr(a)); }
void servo_set4(float a) { TIM_SetCompare2(SERVO_TIM, angle_to_ccr(a)); }

rt_uint8_t servo_get1(void) { return ccr_to_angle(TIM_GetCapture3(SERVO_TIM)); }
rt_uint8_t servo_get2(void) { return ccr_to_angle(TIM_GetCapture1(SERVO_TIM)); }
rt_uint8_t servo_get3(void) { return ccr_to_angle(TIM_GetCapture4(SERVO_TIM)); }
rt_uint8_t servo_get4(void) { return ccr_to_angle(TIM_GetCapture2(SERVO_TIM)); }
