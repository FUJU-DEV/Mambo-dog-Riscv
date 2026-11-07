# 编译与工程搭建指南（A1 修复）

本仓库只提供无工程依赖的 12 个源文件（`firmware/`），需要挂到 RT-Thread 工程里编译。
本文给三条路线，**推荐路线一**。

---

## 路线零：使用 `project/` 覆盖包（最快，推荐）

仓库里的 [`project/`](../project/) 是**配置好的工程覆盖包**（rtconfig.h / rtconfig_preinc.h / link.lds 128K/32K /
board 文件 / SConstruct），按 [project/README.md](../project/README.md) 五步操作即可：

```bash
git clone --depth 1 https://github.com/RT-Thread/rt-thread.git
cd rt-thread/bsp/wch/risc-v/ch32v307v-r1
cp -r <仓库>/project/rtconfig*.h <仓库>/project/rtconfig.py .
cp -r <仓库>/project/board/* board/
cp <仓库>/project/SConstruct <仓库>/project/SConscript .
cp -r <仓库>/project/applications/SConscript applications/
cp <仓库>/firmware/*.c <仓库>/firmware/*.h applications/
rm applications/main.c
scons -j8
```

> 原理：工程文件/启动文件/SDK 都由 RT-Thread 官方 BSP 提供
> （`bsp/wch/risc-v/ch32v307v-r1` + `bsp/wch/risc-v/libraries`，后者内置 WCH SDK），
> 覆盖包只负责把配置改成 CH32V303 正确值（器件宏 `CH32V30x_D8`、128K/32K、关闭串口设备框架）。

## 路线一：RT-Thread Studio（手动建工程）

### 1. 准备

- 安装 [RT-Thread Studio](https://www.rt-thread.org/studio.html)（自带 GCC 工具链与 RT-Thread 源码）
- **RT-Thread 版本：5.1.x 或更高**（BSP `ch32v307v-r1` 在 5.1 起可稳定使用；5.0.x 也可，API 兼容）

### 2. 新建工程

```
文件 → 新建 → RT-Thread 项目 → 基于开发板 → 搜索 "ch32v307" → 选择 CH32V307V-R1 模板
```

（BSP 源码路径：`rt-thread/bsp/wch/risc-v/ch32v307v-r1`）

### 3. rtconfig.h 关键配置

在工程的 `rtconfig.h` 中确认/添加：

```c
/* 必须：动作线程使用邮箱 */
#define RT_USING_MAILBOX

/* 建议：关闭串口设备框架（固件直接操作 USART3 外设，避免与设备驱动抢中断） */
/* #define RT_USING_SERIAL */

/* 版本 5.x 默认已开，无需改动 */
#define RT_USING_HEAP
#define RT_USING_SMP /* 单核时不用改 */

/* 器件宏：CH32V303（不定义时 SDK 默认按 CH32V307 编译） */
#define CH32V30x_D8
```

> ⚠️ **CH32V303 的器件宏 `CH32V30x_D8` 必须定义**（ch32v30x.h 顶部注释：`CH32V30x_D8` = CH32V303x，
> `CH32V30x_D8C` = CH32V307/305/317）。定义位置：rtconfig.h 或工程 → 构建属性 → C/C++ 编译器 → 预定义宏。

### 4. 链接脚本 Link.ld（128K Flash / 32K RAM）

BSP 自带的 link.ld 是按 V307 配置的（256K/64K），改为：

```ld
MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 128K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 32K
}
```

（完整模板见 [rt-thread bsp/wch/risc-v/ch32v307v-r1/link.ld](https://github.com/RT-Thread/rt-thread/tree/master/bsp/wch/risc-v/ch32v307v-r1)，只改 MEMORY 段即可）

### 5. 放入固件源文件

1. 把 [firmware/](../firmware/) 下**全部 12 个 .c/.h** 拷入工程 `applications/`（或新建 `dog/` 目录并在工程里加入编译）；
2. **删除**工程自带的 `applications/main.c`（本固件 `app_dog.c` 已提供 `int main(void)`，重复会链接报错）；
3. 确认 `dog_mode.c` 以 **GBK/GB2312** 保存（MounRiver/Studio 默认编码设置：项目属性 → Resource → Text file encoding → GBK；Studio 里用 UTF-8 也可，但 RT-Thread Studio 编译时需加 `-finput-charset=GBK` 才不报错，见常见问题）。

### 6. 编译

点 **构建**（或命令行）：

```bash
scons --target=eclipse   # 如需生成工程文件
scons -j8                 # 直接编译
```

### 7. 验证编译（等价于我们实测的命令）

```bash
riscv64-unknown-elf-gcc -march=rv32imafc -mabi=ilp32f -DCH32V30x_D8 \
  -I<SDK>/Peripheral/inc -I<SDK>/Core -I<RT-Thread>/include \
  -fsyntax-only firmware/drv_servo.c
```

> 本仓库 8 个 .c 文件已用官方 WCH SDK 头文件 + riscv64 GCC 实测 **全部编译通过**（0 error）。

---

## 路线二：MounRiver Studio（无 RT-Thread 时的裸机替代）

1. MounRiver Studio → 新建工程 → 芯片选 **CH32V303CBT6**；
2. 器件宏选 `CH32V30x_D8`，主频配置 144MHz；
3. **但本固件依赖 RT-Thread**（线程/邮箱/延时 API）——MounRiver 裸机工程需自行移植 RT-Thread，
   或用 RT-Thread Nano 替代（邮箱可用 rt_mb + 定时器模拟）。**不推荐**，路线一更省事。

---

## 常见问题

| 报错 | 原因 | 解决 |
|---|---|---|
| `stdint.h: No such file or directory` | 工具链缺 newlib 头 | 用 RT-Thread Studio 自带工具链，或安装 `gcc-riscv64-unknown-elf` |
| `failure to convert GBK to UTF-8` | GBK 源文件 + 编译器按 UTF-8 处理 | 文件保持 GBK 且编译加 `-finput-charset=GBK`；或把注释转 UTF-8（不影响程序） |
| `undefined reference to 'main'` 或 `multiple definition of 'main'` | 工程自带 main.c 未删 | 删除 `applications/main.c` |
| `undefined reference to 'rt_mb_create'` | RT_USING_MAILBOX 未开 | rtconfig.h 打开 `RT_USING_MAILBOX` |
| 链接报 FLASH 溢出 | 链接脚本没改 128K | 按上文改 link.ld 的 LENGTH |
| 烧录后不跑 / 舵机抖 | 主频不是 144MHz | 改 `dog_config.h` 的 `DOG_TIM_CLK_HZ` 或工程时钟配置 |
