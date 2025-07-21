#include "Common.h"

LRESULT WndProc_Win32(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = 0;

    switch (uMsg)
    {
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            if (wParam == VK_ESCAPE)
            {
                GlobalState.bRunning = false;
            }
        }
        case WM_CLOSE:
        case WM_DESTROY:
        case WM_QUIT:
        {
            GlobalState.bRunning = false;
        } break;
        default:
        {
            lResult = DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
    }

    return lResult;
}

void InitPlatform_Win32(GameState* _State)
{
    int WindowWidth = 1280;
    int WindowHeight = 720;
    RECT WorkArea = {};
    if (SystemParametersInfoA(SPI_GETWORKAREA, 0, &WorkArea, 0))
    {
        WindowWidth = WorkArea.right - WorkArea.left;
        WindowHeight = WorkArea.bottom - WorkArea.top;
    }

    LPCSTR WindowClassName = "UntitledBulletGame";
    DWORD WindowStyle = WS_OVERLAPPEDWINDOW;
    DWORD WindowExStyle = 0;

    WNDCLASSEXA WindowClass = {};
    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WndProc_Win32;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = _State->hInstance;
    WindowClass.lpszClassName = WindowClassName;
    (void)RegisterClassExA(&WindowClass);

    _State->Window = CreateWindowExA(
        WindowExStyle,
        WindowClassName,
        WindowClassName,
        WindowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowWidth, WindowHeight,
        nullptr, nullptr, nullptr, nullptr
    );

    ShowWindow(_State->Window, SW_SHOWNORMAL);
}

void TickPlatform_Win32(GameState* _State)
{
    if (_State && _State->Window)
    {
        MSG Msg = {};
        while (PeekMessageA(&Msg, _State->Window, 0, 0, TRUE))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
    }
}

