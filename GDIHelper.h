#pragma once

extern ULONG g_CurrentColor;
extern BYTE *g_pBits;

void InitGDI(HWND hWnd);
void ReleaseGDI(HWND hWnd);
void SetColor(BYTE r, BYTE g, BYTE b);
void SetColor(ULONG color);
void Clear();
void BufferSwap();
