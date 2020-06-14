#include <Windows.h>
#include "utils.h"


#define MENU_ITEM_TABLE_MODE            20001
#define MENU_ITEM_QUERY_BUILDER_MODE    20002


#define RCX(rc) (rc.left)
#define RCY(rc) (rc.top)
#define RCW(rc) (rc.right - rc.left)
#define RCH(rc) (rc.bottom - rc.top)

namespace g {
    sql::Connection* connection = connect("tcp://127.0.0.1:3306", "root", "1k2o3s4t5y6A");
    sql::SQLString schema = "mydb";

    LPCSTR windowHeader = "MAIN";
    LPCSTR windowClassName = "MainWindow";

}


namespace rect {
    RECT window = { 0, 0, 800, 800 };
    RECT table = { 0, 400, 800, 800 };
    

}

namespace appContext {
    HINSTANCE hInst;

    HWND hwndTable;
    HWND mainWindow;
    HMENU mainWindowMenu;
    HMENU mainWindowSelectModeMenu;



    sql::SQLString requestStr;

    TABLE resultTable;
    std::string resultAnswer;
}




ATOM RegClass(LPCSTR, LPCSTR, WNDPROC, HINSTANCE, UINT, INT, INT);

BOOL StartApp();

VOID ShowLastError(LPCSTR);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
 
    g::connection->setSchema(g::schema);
    appContext::requestStr = sql::SQLString("SELECT * FROM x;");

    appContext::hInst = hInstance;
    if (hPrevInstance) return FALSE;
    if (!StartApp()) return FALSE;

    ShowWindow(appContext::mainWindow, SW_SHOW);
    UpdateWindow(appContext::mainWindow);


    MSG msg;
    BOOL bRet;

    while ((bRet = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            ShowLastError("Error in main loop.");
        }
        else
        {

            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
    }

    return msg.wParam;
}



ATOM RegClass(
    LPCSTR className,
    LPCSTR menuName,
    WNDPROC wndProc,
    HINSTANCE hInst,
    UINT style = 0,
    INT clsExtra = 0,
    INT wndExtra = 0) {
    WNDCLASS newClass;

    memset(&newClass, 0, sizeof(newClass));

    newClass.lpszClassName = className;
    newClass.lpszMenuName = menuName;
    newClass.lpfnWndProc = wndProc;
    newClass.hInstance = hInst;
    newClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    newClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    newClass.cbClsExtra = clsExtra;
    newClass.cbWndExtra = wndExtra;
    newClass.style = style;
    newClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    return RegisterClass(&newClass);
}


BOOL StartApp()
{
    if (RegClass(
        g::windowClassName,
        NULL,
        WndProc,
        appContext::hInst,
        CS_HREDRAW | CS_VREDRAW,
        0,
        0) == 0)
    {
        ShowLastError("Can't register class.");
        return FALSE;
    }


    appContext::mainWindow = CreateWindow(
        g::windowClassName,
        g::windowHeader,
        WS_OVERLAPPEDWINDOW,
        RCX(rect::window), RCY(rect::window),
        RCW(rect::window), RCH(rect::window),
        NULL,
        NULL,
        appContext::hInst,
        NULL
    );
    if (!appContext::mainWindow)
    {
        ShowLastError("Can't create frame Window.");
        return FALSE;
    }

    return TRUE;
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
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    std::string rMsgStr = text + std::string("\nCode: ") + std::to_string(dw) + ".\n" + (LPCSTR)lpMsgBuf;

    MessageBox(NULL, (rMsgStr).c_str(), "ERROR", MB_OK | MB_ICONERROR);
}


LRESULT CALLBACK WndProc(
    HWND hWnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam
) {

    switch (Msg) {
    case WM_CREATE:
    {
        appContext::mainWindowMenu = CreateMenu();

        SetMenu(hWnd, appContext::mainWindowMenu);

        appContext::mainWindowSelectModeMenu = CreatePopupMenu();
        AppendMenu(appContext::mainWindowSelectModeMenu, MF_ENABLED | MF_STRING,
            MENU_ITEM_TABLE_MODE, "TABLE");

        AppendMenu(appContext::mainWindowSelectModeMenu, MF_ENABLED | MF_STRING,
            MENU_ITEM_QUERY_BUILDER_MODE, "QUERY BUILDER");

        AppendMenu(appContext::mainWindowMenu, MF_ENABLED | MF_POPUP,
            (UINT)appContext::mainWindowSelectModeMenu, "MODE");

        DrawMenuBar(hWnd);

    }
    break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    break;
    }



    return DefWindowProc(hWnd, Msg, wParam, lParam);
}