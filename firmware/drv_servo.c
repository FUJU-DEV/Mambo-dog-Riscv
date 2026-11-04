#include "dog.h"

#define SERVO_MIN_CCR  500U   /* 0.5ms -> 0 度 */
#define SERVO_MAX_CCR  2500U  /* 2.5ms -> 180 度 */

static rt_uint16_t angle_to_ccr(float angle)
{
    if (angle <= 0.0f)   return SERVO_MIN_CCR;
    if (angle >= 180.0f) return SERVO_MAX_CCR;
    return (rt_uint16_t)(angle * (float)(SERVO_MAX_CCR - SERVO_MIN_CCR) / 180.0f + SERVO_MIN_CCR);
}

void servo_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef g;
    g.GPIO_Mode  = GPIO_Mode_AF_PP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;   /* PA6=CH1 右前, PA7=CH2 右后 */
    GPIO_Init(GPIOA, &g);
    g.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;   /* PB0=CH3 左前, PB1=CH4 左后 */
    GPIO_Init(GPIOB, &g);

    TIM_TimeBaseInitTypeDef tb;
    tb.TIM_Prescaler         = (rt_uint16_t)SERVO_PSC;
    tb.TIM_CounterMode       = TIM_CounterMode_Up;
    tb.TIM_Period            = SERVO_ARR;
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

    TIM_Cmd(SERVO_TIM, ENABLE);
}

void servo_set1(float a) { TIM_SetCompare1(SERVO_TIM, angle_to_ccr(a)); }
void servo_set2(float a) { TIM_SetCompare2(SERVO_TIM, angle_to_ccr(a)); }
void servo_set3(float a) { TIM_SetCompare3(SERVO_TIM, angle_to_ccr(a)); }
void servo_set4(float a) { TIM_SetCompare4(SERVO_TIM, angle_to_ccr(a)); }

rt_uint8_t servo_get1(void) { return (rt_uint8_t)((TIM_GetCapture1(SERVO_TIM) - SERVO_MIN_CCR) * 180U / (SERVO_MAX_CCR - SERVO_MIN_CCR)); }
rt_uint8_t servo_get2(void) { return (rt_uint8_t)((TIM_GetCapture2(SERVO_TIM) - SERVO_MIN_CCR) * 180U / (SERVO_MAX_CCR - SERVO_MIN_CCR)); }
rt_uint8_t servo_get3(void) { return (rt_uint8_t)((TIM_GetCapture3(SERVO_TIM) - SERVO_MIN_CCR) * 180U / (SERVO_MAX_CCR - SERVO_MIN_CCR)); }
rt_uint8_t servo_get4(void) { return (rt_uint8_t)((TIM_GetCapture4(SERVO_TIM) - SERVO_MIN_CCR) * 180U / (SERVO_MAX_CCR - SERVO_MIN_CCR)); }
