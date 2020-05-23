#include <Windows.h>
#include <string>
#include <sstream>

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID ShowLastError(LPCSTR);

HINSTANCE hInst;
HWND hWndMainClass;
LPCSTR mainClassName = "mainClass";



int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{

    MSG message;
    hInst = hInstance;
    WNDCLASS mainClass;

    mainClass.style = mainClass.cbClsExtra = mainClass.cbWndExtra = 0;
    mainClass.lpfnWndProc = MainWndProc;
    mainClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    mainClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    mainClass.hInstance = hInstance;
    mainClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    mainClass.lpszMenuName = NULL;
    mainClass.lpszClassName = mainClassName;
    if (RegisterClass(&mainClass) == 0) {

        ShowLastError("Can't register class!");
        return FALSE;
    }



    hWndMainClass = CreateWindow(mainClassName, "TestMainWindow",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, hInstance, NULL);


    if (!hWndMainClass) {
        ShowLastError("Can't create window!");
        return FALSE;
    }
    else
        ShowWindow(hWndMainClass, nShowCmd);


    while (GetMessage(&message, 0, 0, 0)) {

        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return message.wParam;
}


LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    static HWND button_reg_class_error, button_create_window_error, button_user_error_create, button_user_error_close;
    WNDCLASS Class1, Class2, Class3;
    HWND hwnd2;
    static HWND hwnd3;
    static BOOL hwnd3created = FALSE;



    switch (Msg) {
    case WM_CREATE:
        button_reg_class_error = CreateWindow("button", "REGISTER_CLASS_ERROR",
            WS_VISIBLE | WS_BORDER | WS_CHILD,
            50, 50, 150, 50, hWnd, NULL, hInst, NULL);
        ShowWindow(button_reg_class_error, SW_NORMAL);

        button_create_window_error = CreateWindow("button", "CREATE_WINDOW_ERROR",
            WS_VISIBLE | WS_BORDER | WS_CHILD,
            50, 100, 150, 50, hWnd, NULL, hInst, NULL);
        ShowWindow(button_create_window_error, SW_NORMAL);

        button_user_error_create = CreateWindow("button", "USER_ERROR_OPEN",
            WS_VISIBLE | WS_BORDER | WS_CHILD,
            50, 150, 150, 50, hWnd, NULL, hInst, NULL);
        ShowWindow(button_user_error_create, SW_NORMAL);

        button_user_error_close = CreateWindow("button", "USER_ERROR_CLOSE",
            WS_VISIBLE | WS_BORDER | WS_CHILD,
            50, 200, 150, 50, hWnd, NULL, hInst, NULL);
        ShowWindow(button_user_error_close, SW_NORMAL);

        break;
    case WM_COMMAND:
        if (button_reg_class_error == (HWND)lParam) {

            Class1.lpfnWndProc = MainWndProc;
            Class1.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            Class1.hCursor = LoadCursor(NULL, IDC_ARROW);
            Class1.hInstance = hInst;
            Class1.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
            Class1.lpszMenuName = NULL;
            Class1.lpszClassName = "cl1";
            if (RegisterClass(&Class1) == 0) {

                ShowLastError("Can't register class!");
            }

        }
        else if (button_create_window_error == (HWND)lParam) {

            Class2.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
            Class2.cbClsExtra = Class2.cbWndExtra = 0;
            Class2.lpfnWndProc = MainWndProc;
            Class2.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            Class2.hCursor = LoadCursor(NULL, IDC_ARROW);
            Class2.hInstance = hInst;
            Class2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
            Class2.lpszMenuName = NULL;
            Class2.lpszClassName = "cl2";
            if (RegisterClass(&Class2) == 0) {

                ShowLastError("Can't register class!");
            }


            hwnd2 = CreateWindow(NULL, "TestWindow",
                WS_OVERLAPPEDWINDOW,
                200, 100, 100, 100,
                hWnd, 0, hInst, NULL);

            if (!hwnd2) {
                ShowLastError("Can't create window!");
            }
        }
        else if (button_user_error_create == (HWND)lParam && !hwnd3created) {

            Class3.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
            Class3.cbClsExtra = Class3.cbWndExtra = 0;
            Class3.lpfnWndProc = DefWindowProc;
            Class3.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            Class3.hCursor = LoadCursor(NULL, IDC_ARROW);
            Class3.hInstance = hInst;
            Class3.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
            Class3.lpszMenuName = NULL;
            Class3.lpszClassName = "cl3";
            if (RegisterClass(&Class3) == 0) {

                ShowLastError("Can't register class!");
            }
            hwnd3 = CreateWindow("cl3", "TestWindow",
                WS_OVERLAPPEDWINDOW,
                200, 100, 100, 100,
                hWnd, 0, hInst, NULL);

            if (!hwnd3) {
                ShowLastError("Can't create window!");
            }
            else {
                ShowWindow(hwnd3, SW_SHOW);
                hwnd3created = TRUE;
            }
        }
        else if (button_user_error_close == (HWND)lParam && hwnd3created) {
            if (CloseWindow(hwnd3) == 0) {
                ShowLastError("Can't close window!");

            }
            hwnd3created = FALSE;

        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);

        return 0;

        break;

    }



    return DefWindowProc(hWnd, Msg, wParam, lParam);
}


VOID ShowLastError(LPCSTR text) {
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_SYSTEM_DEFAULT, SUBLANG_SYS_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    std::string rMsgStr = text + std::string("\nCode: ") + std::to_string(dw) + ".\n" + (LPCSTR)lpMsgBuf;
    LocalFree(lpMsgBuf);

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    rMsgStr += (LPCSTR)lpMsgBuf;
    LocalFree(lpMsgBuf);
    MessageBox(NULL, (rMsgStr).c_str(), "ERROR", MB_OK | MB_ICONERROR);
}