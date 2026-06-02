/**
 * @file main.c (Win32 GDI 版，零依赖)
 * @brief  SCGUI Windows 模拟器 - 用 Win32 GDI 画图，无需 SDL2
 *
 * 编译（MinGW-w64）：
 *   gcc main.c sdl_lcd.c sc_demo_test.c ^
 *       SCGUI/sc_common.c SCGUI/sc_gui.c SCGUI/sc_arc.c ^
 *       SCGUI/sc_compose.c SCGUI/sc_event_task.c ^
 *       SCGUI/sc_keyboard.c SCGUI/sc_lvgl_font.c ^
 *       SCGUI/sc_menu.c SCGUI/sc_transform.c ^
 *       -lgdi32 -luser32 -lkernel32 -o scgui_win.exe
 *
 * 编译（MSVC x64 Native Tools）：
 *   cl main.c sdl_lcd.c sc_demo_test.c ^
 *      SCGUI\sc_common.c SCGUI\sc_gui.c SCGUI\sc_arc.c ^
 *      SCGUI\sc_compose.c SCGUI\sc_event_task.c ^
 *      SCGUI\sc_keyboard.c SCGUI\sc_lvgl_font.c ^
 *      SCGUI\sc_menu.c SCGUI\sc_transform.c ^
 *      user32.lib gdi32.lib kernel32.lib /Fe:scgui_win.exe
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdl_lcd.h"        /* init_SDL2(), SDL_DrawPoint(), SDL_refresh(), SDL_close() */
#include "sc_common.h"
#include "sc_gui.h"
#include "sc_demo_test.h"
#include "sc_event_task.h"   /* sc_send_pos_event(), sc_task_loop(), sc_create_task() */

/* ======== 外部变量 / 函数声明 ======== */
extern lv_font_t lv_font_20;         /* SCGUI/font/ 里生成的字体 */
volatile uint32_t system_tick = 0;  /* SCGUI 内部引用的节拍计数器 */

/* ======== SCGUI 刷新回调：把 PFB 写入 GDI 帧缓冲 ======== */
static void LCD_Refresh_CB(uint16_t x,  uint16_t y,
                           uint16_t w,  uint16_t h,
                           color_t  *color)
{
    for (uint16_t row = 0; row < h; row++) {
        for (uint16_t col = 0; col < w; col++) {
            SDL_DrawPoint(x + col, y + row, color[row * (uint32_t)w + col]);
        }
    }
}

/* ======== 1ms 节拍线程 ======== */
static DWORD WINAPI TickThread(LPVOID lpParam)
{
    (void)lpParam;
    while (1) {
        Sleep(1);
        system_tick++;
    }
    return 0;
}

/* ======== 主函数（WinMain，Win32 程序入口）======== */
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    /* 1. 初始化 Win32 窗口（sdl_lcd.c）*/
    if (init_SDL2() != 1) {
        MessageBox(NULL, "Failed to create window!", "SCGUI Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    /* 2. 启动 1ms 节拍线程 */
    HANDLE hTick = CreateThread(NULL, 0, TickThread, NULL, 0, NULL);
    if (hTick) CloseHandle(hTick);

    /* 3. 初始化 SCGUI */
    sc_gui_init(LCD_Refresh_CB,
                 C_BLACK,    /* 主题底色 */
                 C_WHITE,    /* 前景色   */
                 C_BLACK,    /* 背景色   */
                 &lv_font_20);

    /* 4. 清屏，然后创建 demo 界面 */
    sc_clear(0, 0, SC_SCREEN_WIDTH, SC_SCREEN_HEIGHT, gui->bkc);

    /* 选一个示例（取消注释其中一个）：*/
    /* sc_demo_rect_pfs();                  —— 矩形/填充性能测试   */
    sc_create_task(0, sc_demo_gif_task, 30); /* —— 动画/GIF 示例       */
    /* sc_demo_text();                     —— 文本显示示例         */
    /* sc_demo_compose();                 —— 组合控件示例         */
    /* sc_demo_edit_task(NULL);            —— 编辑框示例           */
    /* sc_demo_dirty_task(NULL);           —— 脏矩形驱动示例       */

    /* 5. 主循环 */
    printf("SCGUI Windows Simulator running...\n");
    printf("Click the window to simulate touch.\n");

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        sc_task_loop(sc_touch_event);
        sc_touch_loop();
        SDL_refresh();
        Sleep(10);   /* ~10ms 延时，降低 CPU 占用 */
    }

    SDL_close();
    return (int)msg.wParam;
}
