#pragma comment (lib, "comctl32")
#include <Windows.h>
#include <string>
#include <sstream>
#include <CommCtrl.h>


#define ID_STATUS (10001)
#define ID_BUTTON_IMAGE (10002)
#define ID_BUTTON_STRING (10003)




HINSTANCE hInst;
HWND hWndMainClass;
HWND status;
LPCSTR mainClassName = "mainClass";
std::string statusText = "TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT TEXT ";

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ImageThread(LPVOID lpParam);
DWORD WINAPI StringThread(LPVOID lpParam);

VOID ShowLastError(LPCSTR);
HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts);

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
        0, 0, NULL, NULL);

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
    static HWND buttonImage, buttonString;
    static HANDLE imageThread, stringThread;
    static DWORD imgThrParams = 0, strThrParams = 0;
    static DWORD imgThrId, strThrId;

    static bool imgThrIsActive = false, strThrIsActive = false;


    switch (Msg) {
    case WM_CREATE:
        
        buttonImage = CreateWindow("button", "IMAGE_ENABLE",
            WS_VISIBLE | WS_BORDER | WS_CHILD,
            10, 10, 200, 40, hWnd, HMENU(ID_BUTTON_IMAGE), hInst, NULL);

        buttonString = CreateWindow("button", "STRING_ENABLE",
            WS_VISIBLE | WS_BORDER | WS_CHILD,
            10, 70, 200, 40, hWnd, HMENU(ID_BUTTON_STRING), hInst, NULL);

        ShowWindow(buttonImage, SW_NORMAL);
        ShowWindow(buttonString, SW_NORMAL);

        status = DoCreateStatusBar(hWnd, ID_STATUS, hInst, 1);

        SetWindowText(status, statusText.c_str());

        imageThread = CreateThread(NULL, 0, ImageThread, &imgThrParams, CREATE_SUSPENDED, &imgThrId);
        stringThread = CreateThread(NULL, 0, StringThread, &strThrParams, CREATE_SUSPENDED, &strThrId);

        

        break;
    case WM_COMMAND:
        
        switch (LOWORD(wParam)) {
        case ID_BUTTON_IMAGE:
        {
            if (imgThrIsActive) {
                SuspendThread(imageThread);
                SetWindowText(buttonImage, "IMAGE_ENABLE");
            }
            else {
                ResumeThread(imageThread);
                SetWindowText(buttonImage, "IMAGE_DISABLE");
            }
            imgThrIsActive = !imgThrIsActive;
        }
        break;
        case ID_BUTTON_STRING:
        {
            if (strThrIsActive) {
                SuspendThread(stringThread);
                SetWindowText(buttonString, "IMAGE_ENABLE");
            }
            else {
                ResumeThread(stringThread);
                SetWindowText(buttonString, "IMAGE_DISABLE");
            }
            strThrIsActive = !strThrIsActive;
        }
        break;

        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);

        return 0;

        break;

    }



    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

DWORD __stdcall ImageThread(LPVOID lpParam)
{
    static int maxImgIndex = 28, curImgIndex = 0;
    while (true) {
        if (curImgIndex == maxImgIndex) {
            curImgIndex = 0;
        }
        else {
            ++curImgIndex;
        }
        std::string imgName = std::to_string(curImgIndex) + ".bmp";

        HANDLE hBitmap = LoadImage(NULL, imgName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        HDC hDC = GetDC(hWndMainClass);
        HDC hMemDC = CreateCompatibleDC(hDC); 
        SelectObject(hMemDC, hBitmap); 
        BitBlt(hDC, 220, 0, 860, 640, hMemDC, 0, 0, SRCCOPY); 
        DeleteDC(hMemDC);
        ReleaseDC(hWndMainClass, hDC);

        Sleep(5);

    }

    return 0;
}

DWORD __stdcall StringThread(LPVOID lpParam)
{
    while (true) {
        statusText.insert(statusText.begin(), statusText.back());
        statusText.pop_back();

        SetWindowText(status, statusText.c_str());
        Sleep(200);
    }


    return 0;
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

HWND DoCreateStatusBar(HWND hwndParent, int idStatus, HINSTANCE hinst, int cParts)
{
    HWND hwndStatus;

    InitCommonControls();

    hwndStatus = CreateWindowEx(
        0,                       
        STATUSCLASSNAME,         
        (PCTSTR)NULL,           
        SBARS_SIZEGRIP |         
        WS_CHILD | WS_VISIBLE,   
        0, 0, 0, 0,              
        hwndParent,              
        (HMENU)idStatus,       
        hinst,                   
        NULL);                   

    
   
    return hwndStatus;
}