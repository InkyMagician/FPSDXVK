#include "fileops.h"
#include <shlobj.h>
#include <stdio.h>
#include "resource.h"
#include <direct.h> // For _wgetcwd
#include <wchar.h> // Include this for wcscmp
#include <windows.h> // Include this for PathFileExistsW and DeleteFileW

wchar_t szGameDir[MAX_PATH] = L"";

void ChooseGameDirectory(HWND hwnd) {
    BROWSEINFOW bi = { 0 };
    bi.lpszTitle = L"Select Game Directory";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);

    if (pidl != 0) {
        SHGetPathFromIDListW(pidl, szGameDir);
        SetWindowTextW(GetDlgItem(hwnd, 1005), szGameDir); // Assuming 1005 is the ID for hWndStatic3
        IMalloc* imalloc = 0;
        if (SUCCEEDED(SHGetMalloc(&imalloc))) {
            imalloc->Free(pidl);
            imalloc->Release();
        }
    }
}

int ExtractRARFile(const wchar_t* rarFileName, const wchar_t* destDir) {
    HRSRC hResource = NULL;
    HGLOBAL hMemory = NULL;
    void* pResourceData = NULL;
    DWORD resourceSize = 0;
    FILE* pFile = NULL;
    int result = 0;

    // Map filename to resource ID
    int resourceID;
    if (wcscmp(rarFileName, L"VulkanNew.rar") == 0) resourceID = IDR_VULKANNEW;
    else if (wcscmp(rarFileName, L"VulkanOld.rar") == 0) resourceID = IDR_VULKANOLD;
    else if (wcscmp(rarFileName, L"60fps.rar") == 0) resourceID = IDR_60FPS;
    else if (wcscmp(rarFileName, L"144fps.rar") == 0) resourceID = IDR_144FPS;
    else if (wcscmp(rarFileName, L"244fps.rar") == 0) resourceID = IDR_244FPS;
    else if (wcscmp(rarFileName, L"Unlimitedfps.rar") == 0) resourceID = IDR_UNLIMITEDFPS;
    else {
        MessageBoxW(NULL, L"Invalid RAR file specified", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    LPWSTR intresource = MAKEINTRESOURCEW(resourceID);
    // Find and load the resource
    hResource = FindResourceW(NULL, intresource, RT_RCDATA);
    if (hResource == NULL) goto cleanup;

    hMemory = LoadResource(NULL, hResource);
    if (hMemory == NULL) goto cleanup;

    pResourceData = LockResource(hMemory);
    if (pResourceData == NULL) goto cleanup;

    resourceSize = SizeofResource(NULL, hResource);

    // Write the resource data to a temporary file
    wchar_t tempPath[MAX_PATH], tempFileName[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);
    GetTempFileNameW(tempPath, L"RAR", 0, tempFileName);

    if (_wfopen_s(&pFile, tempFileName, L"wb") != 0) goto cleanup;
    fwrite(pResourceData, 1, resourceSize, pFile);
    fclose(pFile);
    pFile = NULL;
    // Extract the temporary RAR file using unrar.exe from the current working directory
    wchar_t currentDir[MAX_PATH];
    if (_wgetcwd(currentDir, MAX_PATH) == NULL) {
        MessageBoxW(NULL, L"Failed to get current working directory", L"Error", MB_OK | MB_ICONERROR);
        goto cleanup;
    }

    // Extract the temporary RAR file
    wchar_t command[512];
    _snwprintf_s(command, 512, L"%s\\unrar.exe x -o+ \"%s\" \"%s\"", currentDir, tempFileName, destDir); //added it already
    result = _wsystem(command);

    // Delete the temporary file
    DeleteFileW(tempFileName);

    if (result != 0) {
        wchar_t errorMsg[256];
        _snwprintf_s(errorMsg, 256, L"Failed to extract %s", rarFileName);
        MessageBoxW(NULL, errorMsg, L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    return 1;

cleanup:
    if (pFile) fclose(pFile);
    MessageBoxW(NULL, L"Failed to load resource", L"Error", MB_OK | MB_ICONERROR);
    return 0;
}

void ApplyChanges(HWND hwnd) {
    if (wcslen(szGameDir) == 0) {
        MessageBoxW(hwnd, L"Please select a game directory first.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    int apiIndex = (int)SendMessageW(GetDlgItem(hwnd, 1002), CB_GETCURSEL, 0, 0);
    int fpsIndex = (int)SendMessageW(GetDlgItem(hwnd, 1003), CB_GETCURSEL, 0, 0);

    wchar_t apiChoice[20];
    wchar_t fpsChoice[20];
    SendMessageW(GetDlgItem(hwnd, 1002), CB_GETLBTEXT, apiIndex, (LPARAM)apiChoice);
    SendMessageW(GetDlgItem(hwnd, 1003), CB_GETLBTEXT, fpsIndex, (LPARAM)fpsChoice);

    // Apply Vulkan configuration if selected
    // Extract to the local directory (szGameDir)
    if (wcscmp(apiChoice, L"Vulkan (New GPU)") == 0) {
        if (!ExtractRARFile(L"VulkanNew.rar", szGameDir)) {
            return;
        }
    }
    else if (wcscmp(apiChoice, L"Vulkan (Old GPU)") == 0) {
        if (!ExtractRARFile(L"VulkanOld.rar", szGameDir)) {
            return;
        }
    }

    // Apply DirectX configuration if selected
    // Delete PandoraSaga.exe.local and d3d9.dll in the game directory
    if (wcscmp(apiChoice, L"DirectX") == 0) {
        wchar_t pandoraSagaLocalPath[MAX_PATH];
        wchar_t d3d9DllPath[MAX_PATH];

        // Construct paths to check and delete
        _snwprintf_s(pandoraSagaLocalPath, MAX_PATH, L"%s\\PandoraSaga.exe.local", szGameDir);
        _snwprintf_s(d3d9DllPath, MAX_PATH, L"%s\\d3d9.dll", szGameDir);

        // Delete PandoraSaga.exe.local if it exists
        if (DeleteFileW(pandoraSagaLocalPath)) {
            MessageBoxW(hwnd, L"PandoraSaga.exe.local deleted successfully.", L"Success", MB_OK | MB_ICONINFORMATION);
        }
        else {
            MessageBoxW(hwnd, L"PandoraSaga.exe.local not found.", L"Information", MB_OK | MB_ICONINFORMATION);
        }

        // Delete d3d9.dll if it exists
        if (DeleteFileW(d3d9DllPath)) {
            MessageBoxW(hwnd, L"d3d9.dll deleted successfully.", L"Success", MB_OK | MB_ICONINFORMATION);
        }
        else {
            MessageBoxW(hwnd, L"d3d9.dll not found.", L"Information", MB_OK | MB_ICONINFORMATION);
        }
    }

    // Apply FPS configuration
    // Extract to the data folder within the local directory
    wchar_t dataDir[MAX_PATH];
    _snwprintf_s(dataDir, MAX_PATH, L"%s\\data", szGameDir);

    // Create the data directory if it doesn't exist
    if (!CreateDirectoryW(dataDir, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
        MessageBoxW(hwnd, L"Failed to create data directory.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    wchar_t fpsRarFileName[20];
    if (wcscmp(fpsChoice, L"Unlimited") == 0) {
        wcscpy_s(fpsRarFileName, L"Unlimitedfps.rar");
    }
    else {
        _snwprintf_s(fpsRarFileName, 20, L"%sfps.rar", fpsChoice);
    }

    if (!ExtractRARFile(fpsRarFileName, dataDir)) {
        return;
    }

    MessageBoxW(hwnd, L"Changes applied successfully!", L"Success", MB_OK | MB_ICONINFORMATION);
}
