#pragma once
#include <windows.h>
#include "resource.h"  // Include resource.h for ID definitions

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateUIControls(HWND hwnd);