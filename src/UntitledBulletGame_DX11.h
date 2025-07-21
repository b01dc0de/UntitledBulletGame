#ifndef UNTITLEDBULLETGAME_DX11_H
#define UNTITLEDBULLETGAME_DX11_H

#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>

struct GameState;
void InitGraphics_DX11(GameState* _State);
void Draw_DX11(GameState* _State);

#endif // UNTITLEDBULLETGAME_DX11_H

