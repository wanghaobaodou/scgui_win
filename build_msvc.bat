@echo off
:: build_msvc.bat - MSVC (Visual Studio) 编译 SCGUI Windows 模拟器
:: 用法：在 "x64 Native Tools Command Prompt" 里运行，或直接双击

where cl >nul 2>&1 || (
    echo ERROR: cl.exe not found.
    echo Open "x64 Native Tools Command Prompt" and re-run.
    pause & exit /b 1
)

echo Building with MSVC...
cl /nologo /O2 /W3 /wd4996 ^
    main.c ^
    sdl_lcd.c ^
    sc_demo_test.c ^
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
    /Fe:scgui_win.exe ^
    /D PY32_MCU=0

if %errorlevel% equ 0 (
    echo.
    echo SUCCESS: scgui_win.exe built.
    echo Run: scgui_win.exe
) else (
    echo.
    echo BUILD FAILED. See errors above.
)
pause
