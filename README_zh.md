# SCGUI Windows 学习工程 - 零依赖版

用 Win32 GDI 模拟 2.4寸 LCD 屏幕，无需 SDL2.dll，无需安装任何库，编译即运行。

---

## 一、文件说明

| 文件 | 作用 |
|------|------|
| `main.c` | 程序入口，初始化 SCGUI + 主循环 |
| `sdl_lcd.c/h` | Win32 GDI 屏幕模拟（替代 SDL2）|
| `sc_demo_test.c/h` | SCGUI 官方示例界面 |
| `SCGUI/` | SCGUI 核心源码（已下载）|
| `build_mingw.bat` | MinGW-w64 一键编译 |
| `build_msvc.bat` | MSVC 一键编译 |

---

## 二、准备编译器

### 方案1：MinGW-w64（推荐，轻量）

1. 下载：https://github.com/niXman/mingw-builds/releases
   - 选 `x86_64-posix-seh-rev0`
2. 解压到 `C:\mingw64`
3. 把 `C:\mingw64\bin` 加到系统 PATH
4. 打开 **新的** PowerShell，验证：
   ```powershell
   gcc --version
   ```

### 方案2：MSVC（Visual Studio）

1. 安装 Visual Studio 2022（社区版免费）
2. 勾选「使用 C++ 的桌面开发」
3. 打开 **x64 Native Tools Command Prompt for VS 2022**
4. 在该命令行里进入工程目录，运行 `build_msvc.bat`

---

## 三、编译运行

### MinGW 方式

```powershell
cd C:\Users\Administrator\.qclaw\workspace-agent-822fc5af\scgui_windows
.\build_mingw.bat
.\scgui_win.exe
```

### MSVC 方式

```cmd
:: 在 "x64 Native Tools Command Prompt" 里运行
cd /d C:\Users\Administrator\.qclaw\workspace-agent-822fc5af\scgui_windows
build_msvc.bat
scgui_win.exe
```

运行成功会弹出一个窗口，显示 SCGUI 的示例界面（各种按钮、进度条、文本）。

---

## 四、修改屏幕参数（适配 2.4寸屏）

打开 `SCGUI/sc_common.h`，找到 `PY32_MCU` 宏：

```c
// 方法：改成 0，走 else 分支（通用配置）
#define PY32_MCU 0

// 在 #else 里修改：竖屏 240x320（2.4寸 ST7789 默认）
#define SC_SCREEN_WIDTH  (240)
#define SC_SCREEN_HEIGHT (320)

// 如果想横屏（320x240）：
// #define SC_SCREEN_WIDTH  (320)
// #define SC_SCREEN_HEIGHT (240)
```

修改后重新编译运行。

---

## 五、SCGUI 常用 API 速查

```c
/* 画实心矩形 */
sc_draw_Fill(NULL, 10, 10, 100, 60, C_RED, 255);

/* 画空心矩形（线宽 2）*/
sc_draw_Frame(NULL, 10, 80, 100, 60, 2, C_BLUE, 255);

/* 画圆角矩形 */
sc_draw_Rounded_rect(NULL, 10, 160, 100, 60, 8, 0, C_GREEN, C_BLACK, 255);

/* 画进度条（vol/max = 百分比）*/
sc_draw_Bar(NULL, 10, 240, 200, 20, 5, 0, C_CYAN, C_DARK_GRAY, 60, 100);

/* 清屏 */
sc_clear(0, 0, SC_SCREEN_WIDTH, SC_SCREEN_HEIGHT, C_BLACK);
```

> 注：第一个参数 `dest` 传 `NULL` 表示直接画到屏幕；
> 若用 SCGUI 的 PFB（部分刷新）则传 `&pfb`。

---

## 六、鼠标模拟触摸

在 Windows 模拟器窗口里：
- **点击** → 模拟触摸按下/释放
- **拖拽** → 模拟触摸移动

SCGUI 控件（按钮、滑块等）会正确响应。

---

## 七、对接实物 CH582 + ST7789

当你在 Windows 模拟器上把 UI 调好后，移植到真硬件只需 2 步：

### 1. 替换 `sdl_lcd.c` → `st7789.c`

把 Win32 GDI 版换成 CH582 的 SPI 驱动（我上一轮写给你的 `sc_lcd_st7789.c`）。

### 2. 修改 `main.c`

- 去掉 `WinMain()`，改用 `int main(void)`（CH582 裸机入口）
- 系统初始化：`SetSysClock()` + `init_ST7789_LCD()`
- 主循环去掉 Windows 消息，换成 `DelayMs(10)`

---

## 八、下一步

1. **先编译 Windows 版**，看界面能不能弹出来
2. 把运行截图发我，我帮你调 UI
3. 确认 UI 满意后，我帮你**完整移植到 CH582 + ST7789**

有任何编译错误，直接把错误信息发给我。
