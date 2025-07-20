#include "Common.h"

struct GameState
{
    bool bRunning;
};

GameState GlobalGameState;

struct Win32PlatformState
{
    HINSTANCE hInstance;
    HWND Window;
};

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
                GlobalGameState.bRunning = false;
            }
        }
        case WM_CLOSE:
        case WM_DESTROY:
        case WM_QUIT:
        {
            GlobalGameState.bRunning = false;
        } break;
        default:
        {
            lResult = DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
    }

    return lResult;
}

void InitPlatform_Win32(Win32PlatformState* _PlatformState)
{
    int WindowWidth = 1920;
    int WindowHeight = 1080;
    LPCSTR WindowClassName = "UntitledBulletGame";
    DWORD WindowStyle = WS_OVERLAPPEDWINDOW;
    DWORD WindowExStyle = 0;

    WNDCLASSEXA WindowClass = {};
    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = WndProc_Win32;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = _PlatformState->hInstance;
    WindowClass.lpszClassName = WindowClassName;
    (void)RegisterClassExA(&WindowClass);

    _PlatformState->Window = CreateWindowExA(
        WindowExStyle,
        WindowClassName,
        WindowClassName,
        WindowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowWidth, WindowHeight,
        nullptr, nullptr, nullptr, nullptr
    );
}

void ShowWindow_Win32(Win32PlatformState* _PlatformState)
{
    if (_PlatformState && _PlatformState->Window)
    {
        ShowWindow(_PlatformState->Window, SW_SHOWNORMAL);
    }
}

void TickPlatform_Win32(Win32PlatformState* _PlatformState)
{
    if (_PlatformState && _PlatformState->Window)
    {
        MSG Msg = {};
        while (PeekMessageA(&Msg, _PlatformState->Window, 0, 0, TRUE))
        {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
    }
}

using PlatformState = Win32PlatformState;
PlatformState GlobalPlatformState;

void Init(HINSTANCE _hInst)
{
    UNUSED_VAR(_hInst);
    OutputDebugStringA("UntitledBulletGame -- INIT\n");

    GlobalPlatformState = { _hInst };
    InitPlatform_Win32(&GlobalPlatformState);
    ShowWindow_Win32(&GlobalPlatformState);

    GlobalGameState.bRunning = true;
}

void GameLoop()
{
    while (GlobalGameState.bRunning)
    {
        TickPlatform_Win32(&GlobalPlatformState);
    }
}

void Term()
{
    OutputDebugStringA("UntitledBulletGame -- TERM\n");
}

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    UNUSED_VAR(hInstance);
    UNUSED_VAR(hPrevInstance);
    UNUSED_VAR(lpCmdLine);
    UNUSED_VAR(nShowCmd);

    Init(hInstance);
    GameLoop();
    Term();

    return 0;
}