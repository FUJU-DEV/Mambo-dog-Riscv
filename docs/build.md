# 编译与工程搭建指南（A1 修复）

> ✅ **实测编译通过（2026-11-07）**：riscv64-unknown-elf-gcc 10.2 + picolibc + WCH SDK v2.7，
> `scons` 全量构建成功：`rtthread.bin` = **49,488 字节**，FLASH 占用 **37.76%**（49.5KB/128KB），
> text 48,896 / data 592 / bss 4,800。正式量产请用 WCH RISC-V GCC 8.2（能识别 CH32V 的
> `interrupt("WCH-Interrupt-fast")` 属性；通用 GCC 只报警告，中断入口行为以 WCH 工具链为准）。

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

> ⚠️ **CH32V303 的器件宏 `CH32V30x_D8` 必须定义在编译器预定义里**（`rtconfig.py` 的 `DEVICE`，
> 或 Studio 的 Preprocessor → Defined symbols），**不能只写 rtconfig.h**——否则 system_ch32v30x.c 按 D8C 编译，
> V307 的「×18」常量在 V303 上被解释为 ×2，主频只有 16MHz（舵机 PWM 变 5.56Hz）。
> `project/rtconfig.py` 的 `DEVICE` 已含 `-DCH32V30x_D8` 和 `-ffunction-sections -fdata-sections`，`LFLAGS` 已含 `-lm`。