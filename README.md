# SCGUI Windows 模拟器 - 零依赖版

用 Win32 GDI 模拟 LCD 屏幕，直接编译运行，不需要 SDL2.dll。

## 一、文件结构

```
scgui_windows/
├── main.c              # 主程序（来自 SCGUI 仓库）
├── sdl_lcd.c          # Win32 GDI 版（已替换 SDL2，零依赖）
├── sdl_lcd.h          # 接口头文件
├── sc_demo_test.c/h   # SCGUI 示例界面
├── SCGUI/             # SCGUI 核心源码（已下载）
│   ├── sc_common.c/h
│   ├── sc_gui.c/h
│   ├── sc_arc.c/h
│   ├── sc_compose.c/h
│   ├── sc_event_task.c/h
│   ├── sc_keyboard.c/h
│   ├── sc_lvgl_font.c/h
│   ├── sc_menu.c/h
│   ├── sc_transform.c/h
│   └── lvgl.h
├── build_mingw.bat    # MinGW 编译脚本
├── build_msvc.bat     # MSVC 编译脚本
└── README.md
```

## 二、编译运行

### 方法1：MinGW（推荐，轻量）

1. 安装 MinGW-w64 → 把 `mingw32-make.exe` 加到 PATH
2. 双击运行 `build_mingw.bat`
3. 生成 `scgui_win.exe`，双击运行

```bat
:: build_mingw.bat 内容（已写好）
gcc main.c sdl_lcd.c sc_demo_test.c ^
    SCGUI/sc_common.c ^
    SCGUI/sc_gui.c ^
    SCGUI/sc_arc.c ^
    SCGUI/sc_compose.c ^
    SCGUI/sc_event_task.c ^
    SCGUI/sc_keyboard.c ^
    SCGUI/sc_lvgl_font.c ^
    SCGUI/sc_menu.c ^
    SCGUI/sc_transform.c ^
    -lgdi32 -luser32 -lkernel32 ^
    -o scgui_win.exe
```

### 方法2：MSVC（Visual Studio）

1. 打开 `x64 Native Tools Command Prompt`
2. 运行 `build_msvc.bat`
3. 生成 `scgui_win.exe`

```bat
:: build_msvc.bat 内容（已写好）
cl main.c sdl_lcd.c sc_demo_test.c ^
   SCGUI\sc_common.c ^
   SCGUI\sc_gui.c ^
   SCGUI\sc_arc.c ^
   SCGUI\sc_compose.c ^
   SCGUI\sc_event_task.c ^
   SCGUI\sc_keyboard.c ^
   SCGUI\sc_lvgl_font.c ^
   SCGUI\sc_menu.c ^
   SCGUI\sc_transform.c ^
   user32.lib gdi32.lib kernel32.lib ^
   /Fe:scgui_win.exe
```

## 三、运行效果

- 弹出一个 **320×240**（或 **240×320**）窗口
- 显示 SCGUI 示例界面（各种控件、矩形、进度条）
- 关闭窗口即退出程序

## 四、修改屏幕参数

打开 `SCGUI/sc_common.h`，修改：

```c
// 竖屏 240×320
#define SC_SCREEN_WIDTH  (240)
#define SC_SCREEN_HEIGHT (320)

// 横屏 320×240
#define SC_SCREEN_WIDTH  (320)
#define SC_SCREEN_HEIGHT (240)
```

## 五、键盘事件（模拟触摸）

在 `main.c` 里已经对接了 SDL 事件：
- 按键盘 → 模拟触摸事件 → SCGUI 控件响应
- 你可以改 `get_SDL_Event()` 函数，把键盘按键映射成触摸坐标

## 六、下一步

1. 先编译运行，看窗口能不能弹出来
2. 把运行截图发我，我帮你接实物 CH582 的 ST7789
3. 在 Windows 模拟器上把 UI 调好，再移植到真硬件，效率最高
