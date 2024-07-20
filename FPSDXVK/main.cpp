#include "main.h"
#include "ui.h"
#include <commctrl.h>
#include "resource.h"
#pragma comment(lib, "comctl32.lib")

HWND g_hwndMain = NULL;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    const wchar_t CLASS_NAME[] = L"GameConfigWindow";

    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    // Create the window
    g_hwndMain = CreateWindowExW(
        0,                          // Optional window styles
        CLASS_NAME,                 // Window class
        L"Game Configuration Tool", // Window 
        WS_OVERLAPPEDWINDOW,        // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, // Size and position
        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (g_hwndMain == NULL) {
        return 0;
    }

    CreateUIControls(g_hwndMain);

    ShowWindow(g_hwndMain, nCmdShow);
    UpdateWindow(g_hwndMain);

    // Run the message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}