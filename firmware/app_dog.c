#include "dog.h"

/* RT-Thread 线程划分：
 *   cmd     线程：轮询语音识别串口，把单字符命令投到邮箱
 *   action  线程：从邮箱取命令，执行对应动作（阻塞动作）
 */

static rt_mailbox_t g_mb = RT_NULL;

static void cmd_thread_entry(void *arg)
{
    char ch;
    (void)arg;
    while (1) {
        while (uart_cmd_poll(&ch)) {
            rt_mb_send(g_mb, (rt_ubase_t)(rt_uint8_t)ch);
        }
        rt_thread_mdelay(5);
    }
}

static void action_thread_entry(void *arg)
{
    rt_ubase_t v;
    (void)arg;
    while (1) {
        if (rt_mb_recv(g_mb, &v, RT_WAITING_FOREVER) == RT_EOK) {
            dog_dispatch((char)(rt_uint8_t)v);
        }
    }
}

int main(void)
{
    rt_thread_t t;

    bsp_dog_init();
    dog_boot();

    g_mb = rt_mb_create("dogmb", 16, RT_IPC_FLAG_FIFO);

    t = rt_thread_create("cmd", cmd_thread_entry, RT_NULL, 768, 10, 20);
    if (t) rt_thread_startup(t);

    t = rt_thread_create("action", action_thread_entry, RT_NULL, 2048, 12, 20);
    if (t) rt_thread_startup(t);

    return 0;
}
