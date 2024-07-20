#include "ui.h"
#include "fileops.h"
#include <commctrl.h>
#include "resource.h"
HWND hWndChooseDir, hWndComboAPI, hWndComboFPS, hWndApply;
HWND hWndStatic1, hWndStatic2, hWndStatic3;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        // Initialize common controls
        INITCOMMONCONTROLSEX icex;
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_WIN95_CLASSES;
        InitCommonControlsEx(&icex);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BUTTON_CHOOSE_DIR:
            ChooseGameDirectory(hwnd);
            return 0;
        case ID_BUTTON_APPLY:
            ApplyChanges(hwnd);
            return 0;
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

void CreateUIControls(HWND hwnd) {
    // Create "Choose Directory" button
    hWndChooseDir = CreateWindowW(
        L"BUTTON", L"Choose Game Directory",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 200, 30, hwnd, (HMENU)ID_BUTTON_CHOOSE_DIR,
        GetModuleHandle(NULL), NULL
    );

    // Create static text for API selection
    hWndStatic1 = CreateWindowW(
        L"STATIC", L"Choose API:",
        WS_CHILD | WS_VISIBLE,
        10, 50, 100, 20, hwnd, NULL,
        GetModuleHandle(NULL), NULL
    );

    // Create API combo box
    hWndComboAPI = CreateWindowW(
        L"COMBOBOX", L"",
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        120, 50, 150, 100, hwnd, (HMENU)ID_COMBO_API,
        GetModuleHandle(NULL), NULL
    );
    SendMessageW(hWndComboAPI, CB_ADDSTRING, 0, (LPARAM)L"DirectX");
    SendMessageW(hWndComboAPI, CB_ADDSTRING, 0, (LPARAM)L"Vulkan (New GPU)");
    SendMessageW(hWndComboAPI, CB_ADDSTRING, 0, (LPARAM)L"Vulkan (Old GPU)");
    SendMessageW(hWndComboAPI, CB_SETCURSEL, 0, 0);

    // Create static text for FPS selection
    hWndStatic2 = CreateWindowW(
        L"STATIC", L"Choose FPS Cap:",
        WS_CHILD | WS_VISIBLE,
        10, 90, 100, 20, hwnd, NULL,
        GetModuleHandle(NULL), NULL
    );

    // Create FPS combo box
    hWndComboFPS = CreateWindowW(
        L"COMBOBOX", L"",
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
        120, 90, 100, 100, hwnd, (HMENU)ID_COMBO_FPS,
        GetModuleHandle(NULL), NULL
    );
    SendMessageW(hWndComboFPS, CB_ADDSTRING, 0, (LPARAM)L"60");
    SendMessageW(hWndComboFPS, CB_ADDSTRING, 0, (LPARAM)L"144");
    SendMessageW(hWndComboFPS, CB_ADDSTRING, 0, (LPARAM)L"244");
    SendMessageW(hWndComboFPS, CB_ADDSTRING, 0, (LPARAM)L"Unlimited");
    SendMessageW(hWndComboFPS, CB_SETCURSEL, 0, 0);

    // Create "Apply Changes" button
    hWndApply = CreateWindowW(
        L"BUTTON", L"Apply Changes",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 130, 200, 30, hwnd, (HMENU)ID_BUTTON_APPLY,
        GetModuleHandle(NULL), NULL
    );

    // Create static text for status messages
    hWndStatic3 = CreateWindowW(
        L"STATIC", L"",
        WS_CHILD | WS_VISIBLE,
        10, 170, 380, 60, hwnd, NULL,
        GetModuleHandle(NULL), NULL
    );
}