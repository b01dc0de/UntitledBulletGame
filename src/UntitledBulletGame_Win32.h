#ifndef UNTITLEDBULLETGAME_WIN32_H
#define UNTITLEDBULLETGAME_WIN32_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT WndProc_Win32(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

struct GameState;
void InitPlatform_Win32(GameState* _State);
void TickPlatform_Win32(GameState* _State);

#endif // UNTITLEDBULLETGAME_WIN32_H

