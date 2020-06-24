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

#define SIGNAL_ERROR        10031
#define SIGNAL_SUCCESS      10032



namespace g {
    sql::Connection* connection = connect("tcp://127.0.0.1:3306", "root", "1k2o3s4t5y6A");
    sql::SQLString schema = "lab5";

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
    RECT msgView = { 250, 0, 450, 400 };

}

namespace request {
    std::vector<std::string> tokens;
    std::string table;
    std::string field;
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

    HWND msgView;

    sql::SQLString requestStr;

    TABLE resultTable;
    std::string resultAnswer;
    int resultCode = -1;

   
    
}

static int COLUMNS_COUNT = 0;
static int ROWS_COUNT = 0;

void contextChange(States state);
void changeContextItem(HWND hwnd, char status);
void signalSelector(int lWord, int hWord);


void reset();
void fillMultilineViewFromTokens(HWND& view, std::vector<std::string>& tokens);
void updateRequestStr(std::vector<std::string>& tokens);
void uodateRequestStr(std::string newStr);


ATOM RegClass(LPCSTR, LPCSTR, WNDPROC, HINSTANCE, UINT, INT, INT);

BOOL StartApp();

VOID ShowLastError(LPCSTR);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void resetDataAndMoveToRequestConstructProc(int lWord, int hWord);
void resetDataAndMoveToSelectTableProc(int lWord, int hWord);
void executeRequestProc(int lWord, int hWord);
void successProc(int lWord, int hWord);
void errorProc(int lWord, int hWord);

void startBuildDeleteProc(int lWord, int hWord);
void startBuildSelectProc(int lWord, int hWord);
void startBuildInsertProc(int lWord, int hWord);
void startBuildUpdateProc(int lWord, int hWord);


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
    changeContextItem(appContext::msgView, contextTable[s][(int)UIitems::errorMsg]);
    
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

void signalSelector(int lWord, int hWord)
{
    

    switch (lWord) {
    case BUTTON_OK: 
    {
        appContext::signalNow = Signals::ok;
    } break;
    case BUTTON_NEXT: 
    {
        appContext::signalNow = Signals::next;
    } break;
    case BUTTON_EXECUTE: 
    {
        appContext::signalNow = Signals::execute;
    } break;
    case BUTTON_DELETE:
    {
        appContext::signalNow = Signals::delete_;
    } break;
    case BUTTON_SELECT:
    {
        appContext::signalNow = Signals::select_;
    } break;
    case BUTTON_UPDATE:
    {
        appContext::signalNow = Signals::update;
    } break;
    case BUTTON_INSERT:
    {
        appContext::signalNow = Signals::insert;
    } break;
    case MENU_ITEM_TABLE_MODE:
    {
        appContext::signalNow = Signals::menuTableItem;
    } break;
    case MENU_ITEM_QUERY_BUILDER_MODE:
    {
        appContext::signalNow = Signals::menuRequestItem;
    } break;
    case SIGNAL_ERROR:
    {
        appContext::signalNow = Signals::error;
    } break;
    case SIGNAL_SUCCESS:
    {
        appContext::signalNow = Signals::success;
    } break;
    }

}

void reset()
{
    request::tokens.clear();
}

void fillMultilineViewFromTokens(HWND& view, std::vector<std::string>& tokens)
{
    std::string s;
    for (auto token : tokens) {
        s.append(token);
    }
    SetWindowText(view, s.c_str());
}

void updateRequestStr(std::vector<std::string>& tokens)
{
    std::string s;
    for (auto token : tokens) {
        s.append(token);
    }
    appContext::requestStr = s.c_str();
}

void uodateRequestStr(std::string newStr)
{
    appContext::requestStr = newStr;
    SetWindowText(appContext::multilineEdit, newStr.c_str());
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


        appContext::hwndTable = createTable(ID_TABLE, hWnd, appContext::hInst, rect::table);

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

        appContext::msgView = CreateWindow("Edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY,
            RCX(rect::msgView), RCY(rect::msgView),
            RCW(rect::msgView), RCH(rect::msgView),
            hWnd, NULL, appContext::hInst, 0);

        contextChange(appContext::stateNow);

    }
    break;
    case WM_COMMAND: 
    {
        int lWord = LOWORD(wParam);
        int hWord = HIWORD(wParam);

        signalSelector(lWord, hWord);
        appContext::actionNow = actionTable[(int)appContext::stateNow][(int)appContext::signalNow];

        switch (appContext::actionNow) {
        case Actions::addFieldToRequest:  break;
        case Actions::addFieldToRequestsCondition:break;
        case Actions::clearTable:break;
        case Actions::closeErrorWindow:break;
        case Actions::execRequest: executeRequestProc(lWord, hWord); break;
        case Actions::insertComparisonOperator:break;
        case Actions::insertFieldValue:break;
        case Actions::insertFieldValueInWhere:break;
        case Actions::insertLogicOperator:break;
        case Actions::resetDataAndMoveToRequestConstruct: resetDataAndMoveToRequestConstructProc(lWord, hWord); break;
        case Actions::resetDataAndMoveToSelectTable: resetDataAndMoveToSelectTableProc(lWord, hWord); break;
        case Actions::showError: errorProc(lWord, hWord); break;
        case Actions::showResult: successProc(lWord, hWord); break;
        case Actions::startBuildDelete:  break;
        case Actions::startBuildInsert:break;
        case Actions::startBuildSelect:break;
        case Actions::startBuildUpdate:break;
        case Actions::startBuildWhereSection:break;

        }

        appContext::stateNow = stateMoveTable[(int)appContext::stateNow][(int)appContext::signalNow];
        contextChange(appContext::stateNow);
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

void resetDataAndMoveToRequestConstructProc(int lWord, int hWord)
{
    if (lWord == COMBO_TABLES) {

        switch (hWord) {
        case CBN_SELCHANGE:

            int line = SendMessage(appContext::comboTables, CB_GETCURSEL, 0, 0);

            if (line != CB_ERR) {
                request::table = appContext::resultTable[line][0];
            }
            break;
        }
    }
    else {
        reset();

        appContext::requestStr = "SHOW TABLES;";
        appContext::resultCode = executeRequest(
            g::connection,
            appContext::requestStr,
            appContext::resultAnswer,
            appContext::resultTable
        );
        appContext::requestStr = "";


        if (appContext::resultCode == 0) {
            std::vector<std::string> list;
            for (auto item : appContext::resultTable) {
                list.push_back(item.at(0));
            }

            clearComboBox(appContext::comboTables);
            insertRowsInComboBox(list, appContext::comboTables);
        }
    }
}

void resetDataAndMoveToSelectTableProc(int lWord, int hWord)
{
    if (lWord == COMBO_TABLES) {
        switch (hWord) {
        case CBN_SELCHANGE:

            int line = SendMessage(appContext::comboTables, CB_GETCURSEL, 0, 0);

            if (line != CB_ERR) {
                request::tokens.clear();
                request::table = appContext::resultTable[line+1][0];
                request::tokens.push_back("SELECT * FROM ");
                request::tokens.push_back(request::table);
                request::tokens.push_back(";");
                fillMultilineViewFromTokens(appContext::multilineEdit, request::tokens);
                updateRequestStr(request::tokens);
            }
            break;
        }
    }
    else {
        reset();

        

        appContext::requestStr = "SHOW TABLES;";
        appContext::resultCode = executeRequest(
            g::connection,
            appContext::requestStr,
            appContext::resultAnswer,
            appContext::resultTable
        );
        appContext::requestStr = "";

        if (appContext::resultCode == 0) {
            std::vector<std::string> list;
            for (size_t i = 1; i < appContext::resultTable.size(); ++i) {
                list.push_back(appContext::resultTable[i][0]);
            }

            clearComboBox(appContext::comboTables);
            insertRowsInComboBox(list, appContext::comboTables);
        }
    }
}

void executeRequestProc(int lWord, int hWord)
{
    appContext::resultCode = executeRequest(
        g::connection,
        appContext::requestStr,
        appContext::resultAnswer,
        appContext::resultTable
    );

    appContext::stateNow = stateMoveTable[(int)appContext::stateNow][(int)appContext::signalNow];
    contextChange(appContext::stateNow);

    if (appContext::resultCode == 0) {
        SendMessage(appContext::mainWindow, WM_COMMAND, MAKEWPARAM(SIGNAL_SUCCESS, 0), 0);
    }
    else {
        SendMessage(appContext::mainWindow, WM_COMMAND, MAKEWPARAM(SIGNAL_ERROR, 0), 0);
    }
   
    
}

void successProc(int lWord, int hWord)
{
    if (COLUMNS_COUNT != 0) {
        for (int i = 0; i < COLUMNS_COUNT; i++) {
            ListView_DeleteColumn(appContext::hwndTable, i);
        }
        ListView_DeleteAllItems(appContext::hwndTable);
        for (int i = 0; i < COLUMNS_COUNT; i++) {
            ListView_DeleteColumn(appContext::hwndTable, i);
        }
    }
   
    LVCOLUMN lvC;
    LVITEM lvI;

    memset(&lvI, 0, sizeof(lvI));
    memset(&lvC, 0, sizeof(lvC));


    lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvC.fmt = LVCFMT_LEFT;  
    lvC.cx = 100;            

    ROWS_COUNT = (int)appContext::resultTable.size();
    COLUMNS_COUNT = (int)appContext::resultTable[0].size();

    for (int index = 0; index < COLUMNS_COUNT; index++)
    {
        lvC.pszText = (LPSTR)appContext::resultTable[0][index].c_str();
        lvC.iSubItem = index;
        
        ListView_InsertColumn(appContext::hwndTable, index, &lvC);
    }

    lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
    lvI.state = 0;
    lvI.stateMask = 0;

  
    for (int index = 1; index < ROWS_COUNT; index++)
    {
        lvI.iItem = index - 1;
        lvI.iSubItem = 0;
        
        lvI.pszText = LPSTR_TEXTCALLBACK;
        lvI.cchTextMax = 100;
        lvI.pszText = (LPSTR)appContext::resultTable[index][0].c_str();

        

        ListView_InsertItem(appContext::hwndTable, &lvI);
    }

    for (int index = 1; index < ROWS_COUNT; index++) {
        for (int iSubItem = 1; iSubItem < COLUMNS_COUNT; iSubItem++)
        {
            ListView_SetItemText(appContext::hwndTable,
                index - 1,
                iSubItem,
                (LPSTR)appContext::resultTable[index][iSubItem].c_str());
        }
    }
    appContext::resultTable.clear();
}

void errorProc(int lWord, int hWord)
{
    std::string errorDesc = "ErrCode: " + std::to_string(appContext::resultCode) + "   State: " + appContext::resultAnswer;
    SetWindowText(appContext::msgView, errorDesc.c_str());
    
}

void startBuildDeleteProc(int lWord, int hWord)
{

}

void startBuildSelectProc(int lWord, int hWord)
{
}

void startBuildInsertProc(int lWord, int hWord)
{
}

void startBuildUpdateProc(int lWord, int hWord)
{
}





