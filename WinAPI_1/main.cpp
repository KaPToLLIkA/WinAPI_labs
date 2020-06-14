#include <windows.h>
#include <string>


#define IDM_MOVE 100
#define IDM_HIDE 101


LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;
HWND hWndMainClass;
HWND hWndChildClass;
LPCSTR mainClassName = "mainClass";
int WndH = 0, WndW = 0;
int gMouseX = 0, gMouseY = 0;
int MouseX = 0, MouseY = 0;

RECT WndRect;
BOOL childClosed = 1;
BOOL moveChildWindow = 0;

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
        MessageBox(NULL, "Can't register main class!", "ERROR", MB_OK | MB_ICONERROR);
        return FALSE;
    }



    hWndMainClass = CreateWindow(mainClassName, "TestMainWindow",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, hInstance, NULL);


    if (!hWndMainClass) {
        MessageBox(NULL, "Can't create window!", "ERROR", MB_OK | MB_ICONERROR);
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
    HMENU hMenu;

    switch (Msg) {
    case WM_CREATE:
        WNDCLASS childClass;
        childClass.cbClsExtra = childClass.cbWndExtra = 0;
        childClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        childClass.lpfnWndProc = ChildWndProc;
        childClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        childClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        childClass.hInstance = hInst;
        childClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        childClass.lpszMenuName = NULL;
        childClass.lpszClassName = "childClass";

        if (RegisterClass(&childClass) == 0) {
            MessageBox(NULL, "Can't register child class!", "ERROR", MB_OK | MB_ICONERROR);
            return FALSE;
        }
        
        GetWindowRect(hWnd, &WndRect);
        WndH = GetSystemMetrics(SM_CYSCREEN);
        WndW = GetSystemMetrics(SM_CXSCREEN);

        break;
    case WM_CONTEXTMENU:

        hMenu = CreatePopupMenu();

        AppendMenu(hMenu, MFT_STRING, IDM_HIDE, "HIDE");
        AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
        AppendMenu(hMenu, MFT_STRING, IDM_MOVE, "MOVE");

        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON |
            TPM_TOPALIGN |
            TPM_LEFTALIGN,
            LOWORD(lParam),
            HIWORD(lParam), 0, hWnd, NULL);
        DestroyMenu(hMenu);
        break;

    case WM_MOUSEMOVE:
        GetWindowRect(hWndMainClass, &WndRect);
        MouseX = LOWORD(lParam);
        MouseY = HIWORD(lParam);
        gMouseX = MouseX + WndRect.left;
        gMouseY = MouseY + WndRect.top;

        InvalidateRect(hWndChildClass, NULL, TRUE);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case IDM_MOVE:
            moveChildWindow = 1;
            break;

        case IDM_HIDE:
            DestroyWindow(hWndChildClass);
            childClosed = 1;
            break;
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);

        return 0;

        break;

    case WM_LBUTTONDOWN:
        if (moveChildWindow) {
            SetWindowPos(hWndChildClass, NULL, MouseX, MouseY, 0, 0, SWP_NOSIZE);
            moveChildWindow = 0;
        }

        if (childClosed) {
            hWndChildClass = CreateWindow("childClass", "ChildWindow",
                WS_CHILD | WS_OVERLAPPEDWINDOW | WS_VISIBLE | LBS_NOTIFY,
                200, 200, 200, 60,
                hWnd, NULL, hInst, NULL);


            if (!hWndChildClass) {
                MessageBox(NULL, "Can't create child window!", "ERROR", MB_OK | MB_ICONERROR);
                return FALSE;
            }
            else {

                UpdateWindow(hWndChildClass);
                SetWindowPos(hWndChildClass, NULL, MouseX, MouseY, 0, 0, SWP_NOSIZE);
            }
            childClosed = 0;
        }
        break;

    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);


}


LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
    case WM_DESTROY:
        childClosed = 1;
        break;

    case WM_PAINT:

        PAINTSTRUCT ps;
        HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        HDC hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 0, 0, (std::to_string(gMouseX) + ":" + std::to_string(gMouseY)).c_str(), (std::to_string(gMouseX) + ":" + std::to_string(gMouseY)).size());
        EndPaint(hWnd, &ps);
        break;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}