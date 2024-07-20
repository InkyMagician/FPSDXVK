#pragma once
#include <windows.h>

void ChooseGameDirectory(HWND hwnd);
int CopyFileToDirectory(const char* sourcePath, const char* destDir, const char* fileName);
int ExtractRARFile(const char* rarPath, const char* destDir);
void ApplyChanges(HWND hwnd);

extern wchar_t szGameDir[MAX_PATH];