#include <Windows.h>
#include "utils.h"
#include "table.h"

#define MENU_ITEM_TABLE_MODE            20001
#define MENU_ITEM_QUERY_BUILDER_MODE    20002


#define ID_TABLE        10001

#define BUTTON_OK       10002
#define BUTTON_NEXT     10003
#define BUTTON_EXECUTE  10004
#define BUTTON_SELECT   10005
#define BUTTON_DELETE   10006
#define BUTTON_INSERT   10007
#define BUTTON_UPDATE   10008

#define COMBO_TABLES        10010
#define COMBO_FIELDS        10011
#define COMBO_CONDITIONS    10012
#define COMBO_OPERATORS     10013

#define EDIT                10020
#define MULTILINE           10021


namespace g {
    sql::Connection* connection = connect("tcp://127.0.0.1:3306", "root", "1k2o3s4t5y6A");
    sql::SQLString schema = "mydb";

    LPCSTR windowHeader = "MAIN";
    LPCSTR windowClassName = "MainWindow";

    std::vector<std::string> operators = { "=", "<=>", "<", ">", "<=", ">=", "<>", "!=" };
    std::vector<std::string> conditions = { "AND", "OR", "NOT", "XOR" };
}


namespace rect {
    RECT window = { 0, 0, 800, 800 };
    RECT table = { 0, 400, 800, 800 };
    
    RECT buttonNext = { 50, 20, 200, 40 },
        buttonOk = { 50, 60, 200, 80 },
        buttonExecute = { 50, 100, 200, 120 },
        buttonSelect = { 50, 140, 200, 160 },
        buttonDelete = { 50, 180, 200, 200 },
        buttonInsert = { 50, 220, 200, 240 },
        buttonUpdate = { 50, 260, 200, 280 };


    RECT comboTables = { 250, 20, 450, 220 },
        comboFields = { 250, 60, 450, 260 },
        comboConditions = { 250, 100, 450, 300 },
        comboOperators = { 250, 140, 450, 340 };

    RECT edit = { 250, 260, 450, 280 };
    RECT multiline = { 500, 0, 800, 400 };

}

namespace appContext {
    States stateNow = States::s1;
    Signals signalNow = Signals::emptyS;
    Actions actionNow;

    HINSTANCE hInst;

    HWND hwndTable;
    HWND multilineEdit;

    HWND mainWindow;
    HMENU mainWindowMenu;
    HMENU mainWindowSelectModeMenu;

    HWND buttonNext,
        buttonOk,
        buttonExecute,
        buttonSelect,
        buttonDelete,
        buttonInsert,
        buttonUpdate;

    HWND comboTables,
        comboFields,
        comboOperators,
        comboConditions;

    HWND edit;

    sql::SQLString requestStr;

    TABLE resultTable;
    std::string resultAnswer;
    int resultCode;
}


void contextChange(States state);
void changeContextItem(HWND hwnd, char status);
void actionSelector(WPARAM wParam, LPARAM lParam);

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



void contextChange(States state)
{
    int s = (int)state;
   
    changeContextItem(appContext::buttonOk, contextTable[s][(int)UIitems::buttonOk]);
    changeContextItem(appContext::buttonDelete, contextTable[s][(int)UIitems::buttonDelete]);
    changeContextItem(appContext::buttonExecute, contextTable[s][(int)UIitems::buttonExecute]);
    changeContextItem(appContext::buttonInsert, contextTable[s][(int)UIitems::buttonInsert]);
    changeContextItem(appContext::buttonNext, contextTable[s][(int)UIitems::buttonNext]);
    changeContextItem(appContext::buttonSelect, contextTable[s][(int)UIitems::buttonSelect]);
    changeContextItem(appContext::buttonUpdate, contextTable[s][(int)UIitems::buttonUpdate]);
    changeContextItem(appContext::comboConditions, contextTable[s][(int)UIitems::comboLogicOperators]);
    changeContextItem(appContext::comboOperators, contextTable[s][(int)UIitems::comboConditionsOperators]);
    changeContextItem(appContext::comboFields, contextTable[s][(int)UIitems::comboFields]);
    changeContextItem(appContext::comboTables, contextTable[s][(int)UIitems::comboTables]);
    changeContextItem(appContext::edit, contextTable[s][(int)UIitems::editField]);
    changeContextItem(appContext::hwndTable, contextTable[s][(int)UIitems::resultTable]);
    changeContextItem(appContext::multilineEdit, contextTable[s][(int)UIitems::multilineField]);
   
}

void changeContextItem(HWND hwnd, char status)
{
    switch (status) {
    case '+':
        ShowWindow(hwnd, SW_SHOW);
        EnableWindow(hwnd, true);
        break;
    case '-':
        ShowWindow(hwnd, SW_SHOW);
        EnableWindow(hwnd, false);
        break;
    case ' ':
        ShowWindow(hwnd, SW_HIDE);
        break;
    }
}

void actionSelector(WPARAM wParam, LPARAM lParam)
{
    //actionTable
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
        ShowLastError("Can't create Window.");
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
        g::connection->setSchema(g::schema);
        appContext::requestStr = sql::SQLString("SELECT * FROM x;");

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


        createTable(ID_TABLE, hWnd, appContext::hInst, rect::table);

        // button
        appContext::buttonOk = createButton("OK", BUTTON_OK,
            hWnd, appContext::hInst, rect::buttonOk);

        appContext::buttonNext = createButton("NEXT", BUTTON_NEXT,
            hWnd, appContext::hInst, rect::buttonNext);

        appContext::buttonExecute = createButton("EXECUTE", BUTTON_EXECUTE,
            hWnd, appContext::hInst, rect::buttonExecute);

        appContext::buttonSelect = createButton("SELECT", BUTTON_SELECT,
            hWnd, appContext::hInst, rect::buttonSelect);

        appContext::buttonDelete = createButton("DELETE", BUTTON_DELETE,
            hWnd, appContext::hInst, rect::buttonDelete);

        appContext::buttonInsert = createButton("INSERT", BUTTON_INSERT,
            hWnd, appContext::hInst, rect::buttonInsert);

        appContext::buttonUpdate = createButton("UPDATE", BUTTON_UPDATE,
            hWnd, appContext::hInst, rect::buttonUpdate);

        // combobox
        appContext::comboConditions = createComboBox("CONDITIONS", COMBO_CONDITIONS,
            hWnd, appContext::hInst, rect::comboConditions);

        appContext::comboFields = createComboBox("FIELDS", COMBO_FIELDS,
            hWnd, appContext::hInst, rect::comboFields);

        appContext::comboOperators = createComboBox("OPERATORS", COMBO_OPERATORS,
            hWnd, appContext::hInst, rect::comboOperators);

        appContext::comboTables = createComboBox("TABLES", COMBO_TABLES,
            hWnd, appContext::hInst, rect::comboTables);

        insertRowsInComboBox(g::conditions, appContext::comboConditions);
        insertRowsInComboBox(g::operators, appContext::comboOperators);

        appContext::edit = CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            RCX(rect::edit), RCY(rect::edit), 
            RCW(rect::edit), RCH(rect::edit),
            hWnd, NULL, appContext::hInst, 0);

        appContext::multilineEdit = CreateWindow("Edit", NULL, 
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY,
            RCX(rect::multiline), RCY(rect::multiline),
            RCW(rect::multiline), RCH(rect::multiline),
            hWnd, NULL, appContext::hInst, 0);

        //contextChange(appContext::stateNow);

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