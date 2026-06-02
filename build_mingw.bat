@echo off
:: build_mingw.bat - MinGW-w64 编译 SCGUI Windows 模拟器
:: 用法：双击运行（需先把 MinGW bin 目录加到 PATH）

where gcc >nul 2>&1 || (
    echo ERROR: gcc not found. Please add MinGW bin to PATH.
    pause & exit /b 1
)

echo Building with MinGW...
gcc -std=c99 -O2 ^
    main.c ^
    sdl_lcd.c ^
    sc_demo_test.c ^
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
    -o scgui_win.exe ^
    -D PY32_MCU=0 ^
    -D _CRT_SECURE_NO_WARNINGS

if %errorlevel% equ 0 (
    echo.
    echo SUCCESS: scgui_win.exe built.
    echo Run: scgui_win.exe
) else (
    echo.
    echo BUILD FAILED. See errors above.
)
pause
