#include "dog.h"

static void led_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef g;
    g.GPIO_Mode  = GPIO_Mode_Out_PP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_Pin   = LED_PIN;
    GPIO_Init(LED_PORT, &g);
    GPIO_SetBits(LED_PORT, LED_PIN);   /* 默认熄灭 */
}

void led_toggle(void)
{
    if (GPIO_ReadOutputDataBit(LED_PORT, LED_PIN)) {
        GPIO_ResetBits(LED_PORT, LED_PIN);
    } else {
        GPIO_SetBits(LED_PORT, LED_PIN);
    }
}

void bsp_dog_init(void)
{
    servo_init();
    uart_dog_init();
    OLED_Init();
    led_init();

    /* 时钟自检：晶振未起振 / 器件宏错误（D8 未生效）时主频会静默偏离，
     * 表现为"能跑但舵机错乱、延时不准"。这里主动快闪 LED 报警。 */
    if (SystemCoreClock != 144000000UL)
    {
        for (;;)
        {
            GPIO_ResetBits(LED_PORT, LED_PIN);
            rt_thread_mdelay(80);
            GPIO_SetBits(LED_PORT, LED_PIN);
            rt_thread_mdelay(80);
        }
    }
}
