#include <iostream>
#include <Windows.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

HANDLE rbxMutex;
HANDLE rauMutex;

HWND hWindow;
NOTIFYICONDATA iconData;

HMENU hPopupMenu;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        iconData.cbSize = sizeof(NOTIFYICONDATA);
        iconData.hWnd = hwnd;
        iconData.uID = 1;
        iconData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
        iconData.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1));
        iconData.uCallbackMessage = WM_USER + 1;
        lstrcpy(iconData.szTip, TEXT("rbxaltunlocker"));
        Shell_NotifyIcon(NIM_ADD, &iconData);

        break;

    case WM_DESTROY:
        ReleaseMutex(rbxMutex);
        CloseHandle(rbxMutex);

        ReleaseMutex(rauMutex);
        CloseHandle(rauMutex);

        Shell_NotifyIcon(NIM_DELETE, &iconData);
        PostQuitMessage(0);

        break;

    case WM_USER + 1:
        if (lParam == WM_LBUTTONDOWN || lParam == WM_RBUTTONDOWN)
        {
            hPopupMenu = CreatePopupMenu();
            AppendMenuW(hPopupMenu, MF_STRING, 1, L"Visit GitHub");
            AppendMenuW(hPopupMenu, MF_STRING, 2, L"Exit");

            SetForegroundWindow(hwnd);

            POINT position;
            GetCursorPos(&position);

            int menuItem = TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, position.x, position.y, 0, hwnd, NULL);
            switch (menuItem)
            {
            case 1:
                ShellExecuteA(NULL, "open", "https://github.com/NeoInversion/rbxaltunlocker", NULL, NULL, SW_SHOWNORMAL);
                break;

            case 2:
                DestroyWindow(hWindow);
                break;
            }
        }

        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void CreateRAUConsole()
{
    AllocConsole();
    FILE *out;
    freopen_s(&out, "CONOUT$", "w", stdout);

    SetConsoleTitleA("rbxaltunlocker");
}

void CloseRAUConsole()
{
    fclose(stdout);
    FreeConsole();
}

bool CreateRBXMutex()
{
    rbxMutex = CreateMutexA(NULL, TRUE, "ROBLOX_singletonMutex");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
        return false;

    return true;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    rauMutex = CreateMutexA(NULL, FALSE, "RAUMutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBoxA(NULL, "rbxaltunlocker is already running!", "Error", MB_OK);
        return WM_QUIT;
    }

    if (CreateRBXMutex())
    {
        CreateRAUConsole();
        std::cout << "Enabled. Minimizing to system tray..." << std::endl;
        Sleep(2000);
        CloseRAUConsole();
    }
    else
    {
        MessageBoxA(NULL, "Launch rbxaltunlocker before Roblox!", "Error", MB_OK);
        return WM_QUIT;
    }

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("RAUClass");
    RegisterClass(&wc);

    hWindow = CreateWindowW(L"RAUClass", L"rbxaltunlocker", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
