#include <Windows.h>
#include <gl/GL.h>

void disp()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}

LRESULT CALLBACK WindowProcedure(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    BOOL bRet;
    MSG msg;
    WNDCLASSEXW wc;
    HWND hwndMain;
    if (!hPrevInstance)
    {
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_DBLCLKS | CS_OWNDC;
        wc.lpfnWndProc = WindowProcedure;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = NULL;
        wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
        wc.hbrBackground = NULL;
        wc.lpszMenuName = NULL;
        wc.lpszClassName = L"MainWindowClass";
        wc.hIconSm = NULL;

        if (!RegisterClassExW(&wc))
            return -2;
    }
    hwndMain = CreateWindowExW(WS_EX_ACCEPTFILES, L"MainWindowClass", L"MainWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 320, 240, NULL, NULL, hInstance, NULL);
	if (!hwndMain)
	{
		return -1;
	}
    ShowWindow(hwndMain, SW_SHOW);
    UpdateWindow(hwndMain);

    // Start the message loop. 

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            // handle the error and possibly exit
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Return the exit code to the system. 

    return msg.wParam;
}