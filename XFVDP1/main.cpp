#include <Windows.h>
#include <KHR/khrplatform.h>
#include <gl/GL.h>

#include <iostream>

using namespace std;

/*#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif // !GL_GLEXT_PROTOTYPES*/
#include <gl/glext.h>

#include "../BBBBBrainDumbed/BBBBBrainDumbed.h"
#include "../BBBBBrainDumbed/Parser.h"

void (APIENTRY* glGenBuffers)(GLsizei n, GLuint* buffers);
void (APIENTRY* glBindBuffer)(GLenum target, GLuint buffer);
void (APIENTRY* glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
void (APIENTRY* glEnableVertexAttribArray)(GLuint index);
void (APIENTRY* glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
void (APIENTRY* glDisableVertexAttribArray)(GLuint index);

static BBBBBrainDumbed* bbbbbraindumbed = NULL;

static const GLfloat vertData[] = {
    0.0,0.0,
    0.5,1.0,
    1.0,0.0
};

void disp()
{
    GLuint vbo = 0;
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)NULL);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

bool initGL()
{
    glGenBuffers = (void(APIENTRY *)(GLsizei n, GLuint* arrays))wglGetProcAddress("glGenBuffers");
    if (!glGenBuffers)
    {
        return false;
    }
    glBindBuffer = (void(APIENTRY *)(GLenum target, GLuint buffer))wglGetProcAddress("glBindBuffer");
    if (!glBindBuffer)
    {
        return false;
    }
    glBufferData = (void(APIENTRY *)(GLenum target, GLsizeiptr size, const void* data, GLenum usage))wglGetProcAddress("glBufferData");
    if (!glBufferData)
    {
        return false;
    }
    glEnableVertexAttribArray = (void(APIENTRY*)(GLuint index))wglGetProcAddress("glEnableVertexAttribArray");
    if (!glEnableVertexAttribArray)
    {
        return false;
    }
    glVertexAttribPointer = (void(APIENTRY*)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer))wglGetProcAddress("glVertexAttribPointer");
    if (!glVertexAttribPointer)
    {
        return false;
    }
    glDisableVertexAttribArray = (void(APIENTRY*)(GLuint index))wglGetProcAddress("glDisableVertexAttribArray");
    if (!glDisableVertexAttribArray)
    {
        return false;
    }
    return true;
}

int OpenAssembly(LPWSTR file)
{
    if (bbbbbraindumbed)
    {
        delete bbbbbraindumbed;
    }
    wstring exepath, filepath;
    basic_ifstream<wchar_t> ifs;
    ifs.open(file);
    if (ifs.fail())
    {
        return 1;
    }
    istreambuf_iterator<wchar_t> ifsbegin(ifs), ifsend;
    wstring finput(ifsbegin, ifsend);
    ifs.close();
    list<Token>* tokens = Tokenizer::tokenize(finput, filepath);
    vector<bool> ROM;
    Parser parser(tokens, filepath);
    try
    {
        ROM = parser.parse();
    }
    catch (const ParserError& e)
    {
        wcout << L"Parser error at token:" << e.token.token << L" filename:" << e.token.filename << L" line:" << e.token.line << L" digit:" << e.token.digit << endl << e.what() << endl;
        return 2;
    }
    catch (const runtime_error& e)
    {
        wcout << L"Parser error\n" << e.what() << endl;
        return 3;
    }
    bbbbbraindumbed = new BBBBBrainDumbed();
    bbbbbraindumbed->memory.bakeRom(ROM);
    LARGE_INTEGER qpc0, qpc1, qpf;
    QueryPerformanceFrequency(&qpf);
    QueryPerformanceCounter(&qpc0);
    for (size_t i = 0; i < 342*262*60*60; i++)
    {
        bbbbbraindumbed->P = 0;
        bbbbbraindumbed->execute(71, false);
    }
    QueryPerformanceCounter(&qpc1);
    OutputDebugStringW(to_wstring((double)(qpc1.QuadPart - qpc0.QuadPart) / qpf.QuadPart).c_str());
    return 0;
}

HCURSOR hcArrow;

LRESULT CALLBACK WindowProcedure(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    HDC hdc;
    HGLRC hglrc;
    PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 24 };
    pfd.iLayerType = PFD_MAIN_PLANE;
    int iCpf;
    BOOL bRet;
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
        case 0:
            switch (LOWORD(wParam))
            {
            case 1:
                break;
            case 2:
            {
                wchar_t filename[MAX_PATH];
                filename[0] = L'\0';
                OPENFILENAMEW ofn = { sizeof(OPENFILENAMEW), hwnd, NULL, L"All Files\0*.*\0\0", NULL, NULL, NULL, /*lpstrFile*/ filename, MAX_PATH, NULL, NULL, NULL, NULL, /*Flags*/ OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
                if (GetOpenFileNameW(&ofn))
                {
                    OpenAssembly(ofn.lpstrFile);
                }
                break;
            }
            default:
                break;
            }
            break;
        case 1:
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
        hdc = GetDC(hwnd);
        iCpf = ChoosePixelFormat(hdc, &pfd);
        if (iCpf == 0)
        {
            PostQuitMessage(GetLastError());
        }
        bRet = SetPixelFormat(hdc, iCpf, &pfd);
        if (bRet == FALSE)
        {
            PostQuitMessage(GetLastError());
        }
        hglrc = wglCreateContext(hdc);
        if (!hglrc)
        {
            PostQuitMessage(GetLastError());
            break;
        }
        bRet = wglMakeCurrent(hdc, hglrc);
        if (bRet == FALSE)
        {
            PostQuitMessage(GetLastError());
        }
        if (!initGL())
        {
            PostQuitMessage(GetLastError());
        }
        disp();
        SwapBuffers(hdc);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hglrc);
        ReleaseDC(hwnd, hdc);
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
    hcArrow = LoadCursor(hInstance, IDC_ARROW);
    if (!hPrevInstance)
    {
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_DBLCLKS | CS_OWNDC;
        wc.lpfnWndProc = WindowProcedure;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = NULL;
        wc.hCursor = hcArrow;
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

    HMENU menuMain = CreateMenu();
    HMENU menuFileParent = CreatePopupMenu();

    AppendMenuW(menuFileParent, MF_GRAYED, 1, L"Open ROM Image");
    AppendMenuW(menuFileParent, 0, 2, L"Open Assembly File");

    AppendMenuW(menuMain, MF_POPUP, (UINT_PTR)menuFileParent, L"File");
    if (!SetMenu(hwndMain, menuMain))
    {
        return -2;
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