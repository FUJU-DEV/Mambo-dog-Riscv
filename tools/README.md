# 烧录工具与编译环境

本目录随仓库提供 **WCH-Link 烧录工具与驱动**（官方工具的 GitHub 镜像版），以及完整的烧录/编译指引。

## 目录内容

| 文件 | 说明 |
|---|---|
| `WCH-LinkUtility.exe` | **WCH-Link 烧录工具**（官方 V1.x，GUI）：连接 WCH-Link 后可直接烧录 .hex/.bin，支持芯片配置字 |
| `WCHLinkDrv_WHQL_S.exe` | WCH-Link 驱动安装（WHQL 签名版，Win10/11 推荐） |
| `CH372DRV_S.exe` | WCH-Link 驱动安装（旧版/备用） |

> 以上三个文件来自官方工具镜像仓库 [chipxatech/WCH-LinkUtility](https://github.com/chipxatech/WCH-LinkUtility)（WCH 官方工具的分发镜像），原版可在 [wch.cn 下载页](https://www.wch.cn/downloads/WCH-LinkUtility_ZIP.html) 获取。

## 需要的全部工具清单

| 工具 | 用途 | 获取方式 |
|---|---|---|
| **WCH-LinkUtility.exe** | SWD 烧录（本目录已提供） | 本仓库 `tools/` |
| **WCH-Link 驱动** | 识别 WCH-Link 硬件 | 本仓库 `tools/`（两个驱动任装其一） |
| **WCH-LinkE 硬件** | 调试器/烧录器（USB 转 SWD） | 淘宝/立创商城搜 "WCH-LinkE"（约 ¥20） |
| **MounRiver Studio** | 官方 IDE（编译+一键下载，自带 RISC-V GCC 与 SDK） | https://www.mounriver.com/download |
| **WCHISPTool** | USB/串口 ISP 烧录（无需 WCH-Link，BOOT0 拉高进入） | https://www.wch.cn/downloads/WCHISPTool_Setup.html |
| **ch32v30x SDK** | 外设库/启动文件/示例（MounRiver 已内置） | https://github.com/openwch/ch32v307 （或 MounRiver 内置） |
| **wchisp**（可选） | 开源命令行烧录器（**USB/UART ISP，不是 SWD**；SWD 用 WCH-LinkUtility） | https://github.com/ch32-rs/wchisp |

## 烧录方法（任选其一）

### 方法 1：WCH-Link + WCH-LinkUtility（推荐）

**接线**（WCH-Link 与 CH32V303CBT6）：

| WCH-Link 引脚 | 接到 CH32V303 | 说明 |
|---|---|---|
| 3V3 | 3.3V（VDD 24/36/48） | 供电（目标板已供电可不接） |
| GND | GND（VSS 23/35/47） | 必须共地 |
| SWDIO | PA13（第 34 脚） | 数据 |
| SWCLK | PA14（第 37 脚） | 时钟 |

**步骤**：
1. 安装 `tools/WCHLinkDrv_WHQL_S.exe`（装完拔插一次 WCH-Link，设备管理器出现 WCH-Link 设备）；
2. 打开 `WCH-LinkUtility.exe` → 芯片选择 **CH32V303**；
3. 点 "Read Chip Info" 能读出芯片信息说明连接成功；
4. "Open File" 选择编译出的 `.hex` 或 `.bin` → "Download" 烧录；
5. 烧录后按一下 RESET（或断电重启）。

### 方法 2：MounRiver Studio 一键下载

1. MounRiver 里打开工程（见根目录 README 快速开始）；
2. 工具栏选择 WCH-Link，点 **Download** 按钮（无需额外配置）；
3. 编译+烧录+复位一条龙。

### 方法 3：USB ISP（无 WCH-Link，WCHISPTool）

1. 把 **BOOT0（第 44 脚）拉高到 3.3V**（板上若无跳线，焊一个排针短接）；
2. 用 USB-C 数据线连接板子（CH32V303 的 USB 在 PA11/PA12，即 D-/D+）；
3. 打开 WCHISPTool → 芯片系列选 CH32V30x → 连接 → 选 .hex → 下载；
4. 烧录后把 BOOT0 恢复低电平再上电。

### 方法 4：命令行 wchisp（可选，CI/批处理）

```bash
# 安装 Rust 后：cargo install wchisp
wchisp flash firmware.hex        # USB/UART ISP（BOOT0 拉高，非 SWD）
wchisp --help
```

## 常见问题

| 现象 | 排查 |
|---|---|
| WCH-LinkUtility 连不上 | ① 驱动装了吗（设备管理器看黄色感叹号）② SWDIO/SWCLK 是否接反 ③ VDD 是否 3.3V 供电 ④ BOOT0 是否误拉高 |
| 烧录成功但程序不跑 | ① 主频是否 144MHz（定时器/串口全依赖）② 链接脚本是否 128K Flash/32K RAM ③ 是否误烧到别的芯片型号 |
| 提示芯片型号不匹配 | 芯片选择 CH32V303（不是 CH32V307）；工程里器件宏用 CH32V30x_D8 |
| SWD 被占用 | 本固件未占用 PA13/PA14；若烧过其他程序，按住 RESET 松手瞬间点 Download 或 BOOT0 拉高进 ISP |
