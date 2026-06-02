/**
 * @file sdl_lcd.c (Win32 GDI 版)
 * @brief 用 Windows GDI 模拟 LCD 屏幕，零依赖，直接编译运行
 * @note  完全替代原 SDL2 版本，接口保持不变，SCGUI 不用改任何代码
 *
 * 编译（MinGW）：
 *   gcc main.c sdl_lcd.c SCGUI/*.c -l gdi32 -l user32 -l kernel32 -o scgui_win.exe
 *
 * 编译（MSVC）：
 *   cl main.c sdl_lcd.c SCGUI\sc_common.c SCGUI\sc_gui.c ... user32.lib gdi32.lib
 */

#include <windows.h>
#include "sc_common.h"
#include "sc_event_task.h"   /* sc_send_pos_event() */
#include "sc_gui.h"

HWND  g_hwnd   = NULL;  /* 导出给 main.c 用 */
static HDC   g_hdc     = NULL;
static uint16_t *g_fb   = NULL;   // RGB565 帧缓冲
static int   g_width    = SC_SCREEN_WIDTH;
static int   g_height   = SC_SCREEN_HEIGHT;
static int   g_scale    = 1;      // 放大倍数（小屏可设为 2）

/* ==================== 窗口过程 ==================== */
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (g_fb) {
            BITMAPINFO bmi = {0};
            bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth       = g_width;
            bmi.bmiHeader.biHeight      = -g_height; /* top-down */
            bmi.bmiHeader.biPlanes      = 1;
            bmi.bmiHeader.biBitCount    = 16;
            bmi.bmiHeader.biCompression = BI_BITFIELDS;
            ((DWORD*)bmi.bmiColors)[0] = 0xF800;
            ((DWORD*)bmi.bmiColors)[1] = 0x07E0;
            ((DWORD*)bmi.bmiColors)[2] = 0x001F;
            StretchDIBits(hdc, 0, 0, g_width * g_scale, g_height * g_scale,
                          0, 0, g_width, g_height,
                          g_fb, &bmi, DIB_RGB_COLORS, SRCCOPY);
        }
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN: {
        int sx = GET_X_LPARAM(lParam) / g_scale;
        int sy = GET_Y_LPARAM(lParam) / g_scale;
        sc_send_pos_event(SC_EVENT_TOUCH_DOWN, sx, sy);
        break;
    }
    case WM_LBUTTONUP: {
        int sx = GET_X_LPARAM(lParam) / g_scale;
        int sy = GET_Y_LPARAM(lParam) / g_scale;
        sc_send_pos_event(SC_EVENT_TOUCH_UP, sx, sy);
        break;
    }
    case WM_MOUSEMOVE: {
        if (wParam & MK_LBUTTON) {
            int sx = GET_X_LPARAM(lParam) / g_scale;
            int sy = GET_Y_LPARAM(lParam) / g_scale;
            sc_send_pos_event(SC_EVENT_TOUCH_MOVE, sx, sy);
        }
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

/* ==================== 初始化窗口 ==================== */
int init_SDL2(void)
{
    // 根据屏幕大小自动选择放大倍数
    g_scale = (g_width < 320) ? 2 : 1;

    HINSTANCE hInstance = GetModuleHandle(NULL);

    const char *cls = "SCGUI_WinClass";
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BLACKBRUSH);
    wc.lpszClassName = cls;
    RegisterClass(&wc);

    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
    g_hwnd = CreateWindow(cls, "SCGUI Windows Simulator",
                          style, CW_USEDEFAULT, CW_USEDEFAULT,
                          g_width * g_scale, g_height * g_scale,
                          NULL, NULL, hInstance, NULL);
    if (!g_hwnd) return -1;

    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);

    // 分配帧缓冲
    g_fb = (uint16_t *)calloc(g_width * g_height, sizeof(uint16_t));
    if (!g_fb) return -1;

    g_hdc = GetDC(g_hwnd);
    printf("SCGUI: %dx%d window created (scale=%d)\n", g_width, g_height, g_scale);
    return 1;
}

/* ==================== 画点（SCGUI 回调）==================== */
void SDL_DrawPoint(uint16_t x, uint16_t y, uint16_t c)
{
    if (x >= SC_SCREEN_WIDTH || y >= SC_SCREEN_HEIGHT) return;
    g_fb[y * g_width + x] = c;
}

/* ==================== 刷新到屏幕 ==================== */
void SDL_refresh(void)
{
    if (!g_hwnd) return;
    InvalidateRect(g_hwnd, NULL, FALSE);
    // 处理 Windows 消息（键盘/鼠标事件）
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

/* ==================== 关闭 ==================== */
void SDL_close(void)
{
    if (g_fb) { free(g_fb); g_fb = NULL; }
    if (g_hwnd) { DestroyWindow(g_hwnd); g_hwnd = NULL; }
    UnregisterClass("SCGUI_WinClass", GetModuleHandle(NULL));
}

/* ==================== 事件处理（供 SCGUI main.c 用）==================== */
int get_SDL_Event(void)
{
    // 在 SDL_refresh() 里已经处理了消息
    return 0;
}
