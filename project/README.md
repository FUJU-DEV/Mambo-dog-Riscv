# 工程覆盖包（让仓库可直接编译）

本仓库**不重复堆放** RT-Thread 内核与 WCH SDK（约 10MB，且官方持续更新），
而是提供一套**配置好的覆盖文件**：覆盖到 RT-Thread 官方 BSP 上即可得到可编译工程。

> 缺的"工程文件 / 链接脚本 / 启动文件 / SDK"来源：
> - **启动文件 / 链接脚本 / 工程文件**：RT-Thread BSP `bsp/wch/risc-v/ch32v307v-r1`
> - **WCH SDK / 外设库**：RT-Thread 仓库内 `bsp/wch/risc-v/libraries`（内置 WCH 官方 SDK，BSP 的 SConstruct 会自动引用）

## 目录说明

| 文件 | 作用 |
|---|---|
| `rtconfig.h` | RT-Thread 配置：**关闭 SERIAL/CONSOLE**（固件直接操作 USART3）、保留 MAILBOX、TICK 1000Hz |
| `rtconfig_preinc.h` | 预包含：**器件宏 `CH32V30x_D8`**（CH32V303，不定义则默认按 V307 编译） |
| `rtconfig.py` | 工具链：`riscv-none-embed-`（RT-Thread Studio 自带）或改 `EXEC_PATH` 为你的 RISC-V GCC |
| `board/board.c` / `board.h` / `software_irq.c` | 官方板级文件（Systick、堆初始化） |
| `board/linker_scripts/link.lds` | **已改为 128K Flash / 32K RAM**（CH32V303CBT6） |
| `SConstruct` / `SConscript` / `applications/SConscript` | 官方构建脚本 |

## 使用步骤（5 分钟）

### 1. 获取 RT-Thread 源码

```bash
git clone --depth 1 https://github.com/RT-Thread/rt-thread.git
cd rt-thread/bsp/wch/risc-v/ch32v307v-r1
```

（或用 RT-Thread Studio 的 BSP 目录；国内可用 Gitee 镜像：`https://gitee.com/rtthread/rt-thread`）

### 2. 覆盖工程文件

把本 `project/` 目录下的内容**覆盖**到当前 BSP 目录：

```bash
cp -r <仓库>/project/rtconfig.h            .
cp -r <仓库>/project/rtconfig_preinc.h     .
cp -r <仓库>/project/rtconfig.py           .
cp -r <仓库>/project/board/*               board/
cp -r <仓库>/project/applications/SConscript applications/
cp <仓库>/project/SConstruct               .
cp <仓库>/project/SConscript               .
```

### 3. 放入固件源码

```bash
cp <仓库>/firmware/*.c <仓库>/firmware/*.h applications/
rm applications/main.c        # 删除 BSP 自带 main（app_dog.c 已提供 main）
```

### 4. 配置工具链并编译

```bash
# 方式 A：用 RT-Thread Studio 自带工具链（默认路径已在 rtconfig.py，按需改 EXEC_PATH）
# 方式 B：用任意 riscv gcc（riscv-none-embed 前缀，或修改 rtconfig.py 的 PREFIX）
export RTT_EXEC_PATH=/path/to/riscv/bin
scons -j8
```

产物：`rtthread.bin` / `rtthread.elf`（rtconfig.py 已配置 objcopy 生成 bin）。

### 5. 烧录

用仓库 [tools/](../tools/) 里的 WCH-LinkUtility 烧录 `rtthread.bin`（芯片选 **CH32V303**）。

## 注意

- `rtconfig.py` 里 `DEVICE` 参数为 `-march=rv32imac -mabi=ilp32`（官方 BSP 默认，软浮点）——
  CH32V303 硬件浮点未启用不影响本固件（舵机角度计算走软浮点，性能足够）；
- 若你的工具链前缀不是 `riscv-none-embed-`（如 `riscv64-unknown-elf-`），改 `rtconfig.py` 的 `PREFIX`；
- 链接脚本已按 CH32V303 改为 **128K/32K**，不要再改回 224K/96K；
- 主频：WCH 库默认 144MHz（V303/V307 一致），固件 `dog_config.h` 的 `DOG_TIM_CLK_HZ` 无需改。
