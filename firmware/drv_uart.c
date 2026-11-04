#include "dog.h"

/* 一个串口（语音识别）轮询收发，不用中断：
 * 命令协议是单字符，语音每秒最多几个字节，轮询完全够用，
 * 也避免了 RISC-V 中断向量/工具链 attribute 的坑。 */

static void uart_asr_init(void)  /* USART3 PB10 TX / PB11 RX 9600 */
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef g;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    g.GPIO_Mode  = GPIO_Mode_AF_PP;
    g.GPIO_Pin   = GPIO_Pin_10;
    GPIO_Init(GPIOB, &g);                 /* PB10 TX */
    g.GPIO_Mode  = GPIO_Mode_IPU;
    g.GPIO_Pin   = GPIO_Pin_11;
    GPIO_Init(GPIOB, &g);                 /* PB11 RX */

    USART_InitTypeDef u;
    u.USART_BaudRate            = UART_ASR_BAUD;
    u.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    u.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    u.USART_Parity              = USART_Parity_No;
    u.USART_StopBits            = USART_StopBits_1;
    u.USART_WordLength          = USART_WordLength_8b;
    USART_Init(UART_ASR, &u);
    USART_Cmd(UART_ASR, ENABLE);
}

void uart_dog_init(void)
{
    uart_asr_init();
}

/* 轮询语音识别，有字节返回 RT_TRUE */
rt_bool_t uart_cmd_poll(char *ch)
{
    if (USART_GetFlagStatus(UART_ASR, USART_FLAG_RXNE) != RESET) {
        *ch = (char)USART_ReceiveData(UART_ASR);
        return RT_TRUE;
    }
    return RT_FALSE;
}
