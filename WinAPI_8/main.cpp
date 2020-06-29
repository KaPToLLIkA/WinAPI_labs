
#include <Windows.h>
#include <string>
#include <vector>



#define ID_BUTTON_START (10002)
#define ID_BUTTON_USER_FIELD_BASE (20000)
#define ID_BUTTON_ENEMY_FIELD_BASE (30000)

enum class FCStatus {
    empty________,
    shipPart_____,
    destroyedPart,
    noDestroyedPt,
};

enum class Team {
    enemy,
    user,
};

namespace g {
    HINSTANCE hInst;
    HWND hWndMainWindow;
    LPCSTR mainClassName = "mainClass";

    const char* emptyCell = " ";
    const char* shipPartCell = "@";
    const char* destroyedPartCell = "X";
    const char* noDestroyedPartCell = "O";

    const size_t MAX_ROWS = 10;
    const size_t MAX_COLS = 10;
    const size_t MAX_SIZE = MAX_COLS * MAX_ROWS;
    const int CELL_H = 50, CELL_W = 50;
    const COORD START_CELL_1 = { 10, 0 }, START_CELL_2 = { 20 + MAX_COLS * CELL_W, 0 };


#define S (FCStatus::empty________)
#define O (FCStatus::shipPart_____)
#define X (FCStatus::destroyedPart)

    const std::vector<FCStatus> defaultField1 = { // 10 x 10
        S,O,O,O,O,S,O,O,O,S,
        S,S,S,S,S,S,S,S,S,S,
        O,S,S,S,S,S,S,S,S,O,
        S,S,S,S,S,S,S,S,S,O,
        O,S,S,S,S,S,S,S,S,S,
        O,S,S,S,S,S,S,S,S,O,
        S,S,S,S,S,S,S,S,S,S,
        O,S,S,S,S,S,S,S,S,S,
        S,S,S,S,S,S,S,S,S,S,
        S,O,O,S,O,S,O,O,O,S,
    };

    const std::vector<FCStatus> defaultField2 = { // 10 x 10
        S,O,O,O,O,S,S,S,S,S,
        S,S,S,S,S,S,S,S,S,O,
        O,S,S,S,S,S,O,S,S,O,
        O,S,S,O,S,S,S,S,S,S,
        O,S,S,S,S,S,S,S,S,S,
        S,S,S,S,S,S,O,S,S,S,
        S,S,O,S,S,S,S,S,S,S,
        S,S,S,S,S,S,S,S,S,O,
        S,S,S,S,S,S,S,S,S,O,
        S,O,O,O,S,S,O,O,S,S,
    };

#undef S
#undef O
#undef X
};


const char* getCellTextFromStatus(FCStatus& status, Team team);
bool findAnyShip(std::vector<FCStatus>& statusField);

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

    {
        g::hInst = hInstance;
        WNDCLASS mainClass;

        mainClass.style = mainClass.cbClsExtra = mainClass.cbWndExtra = 0;
        mainClass.lpfnWndProc = MainWndProc;
        mainClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        mainClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        mainClass.hInstance = hInstance;
        mainClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        mainClass.lpszMenuName = NULL;
        mainClass.lpszClassName = g::mainClassName;

        if (RegisterClass(&mainClass) == 0) {

            ShowLastError("Can't register class!");
            return FALSE;
        }
    }
    
    g::hWndMainWindow = CreateWindow(g::mainClassName, "TestMainWindow",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, NULL, NULL);

    if (!g::hWndMainWindow) {
        ShowLastError("Can't create window!");
        return FALSE;
    }
    else {
        ShowWindow(g::hWndMainWindow, nShowCmd);
    }


    while (GetMessage(&message, 0, 0, 0)) {

        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return message.wParam;
}


const char* getCellTextFromStatus(FCStatus& status, Team team)
{
    switch (status)
    {
    case FCStatus::empty________: return g::emptyCell; break;
    case FCStatus::destroyedPart: return g::destroyedPartCell; break;
    case FCStatus::noDestroyedPt: return g::noDestroyedPartCell; break;
    case FCStatus::shipPart_____: 
        switch (team) {
        case Team::enemy:
            return g::emptyCell;
            break;
        case Team::user:
            return g::shipPartCell;
            break;
        }
        break;
    }
}

bool findAnyShip(std::vector<FCStatus>& statusField)
{
    for (FCStatus s : statusField) {
        if (s == FCStatus::shipPart_____) {
            return true;
        }
    }
    return false;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    static HWND buttonStart;
    static std::vector<FCStatus> userFieldStatus, enemyFieldStatus;
    static std::vector<HWND> userField, enemyField;


    switch (Msg) {
    case WM_CREATE:

        buttonStart = CreateWindow("button", "START_GAME",
            WS_VISIBLE | WS_BORDER | WS_CHILD,
            10, 10, 200, 40, hWnd, HMENU(ID_BUTTON_START), g::hInst, NULL);

        ShowWindow(buttonStart, SW_NORMAL);

        break;
    case WM_COMMAND:

        if (LOWORD(wParam) == ID_BUTTON_START) {
            
            userFieldStatus = std::vector<FCStatus>(g::defaultField1);
            enemyFieldStatus = std::vector<FCStatus>(g::defaultField2);

            COORD userFStartPos = g::START_CELL_1;
            COORD enemyFStartPos = g::START_CELL_2;

            for (size_t i = 0; i < g::MAX_SIZE; ++i) {
                if (i % g::MAX_COLS == 0) {
                    userFStartPos.Y += g::CELL_H;
                    enemyFStartPos.Y += g::CELL_H;

                    userFStartPos.X = g::START_CELL_1.X;
                    enemyFStartPos.X = g::START_CELL_2.X;
                }

                userField.push_back(
                    CreateWindow("button", getCellTextFromStatus(userFieldStatus[i], Team::user),
                        WS_VISIBLE | WS_BORDER | WS_CHILD,
                        userFStartPos.X, userFStartPos.Y, g::CELL_W, g::CELL_H, hWnd,
                        HMENU(ID_BUTTON_USER_FIELD_BASE + i), g::hInst, NULL)
                );
                ShowWindow(userField[i], SW_SHOW);
                EnableWindow(userField[i], false);

                enemyField.push_back(
                    CreateWindow("button", getCellTextFromStatus(enemyFieldStatus[i], Team::enemy),
                        WS_VISIBLE | WS_BORDER | WS_CHILD,
                        enemyFStartPos.X, enemyFStartPos.Y, g::CELL_W, g::CELL_H, hWnd,
                        HMENU(ID_BUTTON_ENEMY_FIELD_BASE + i), g::hInst, NULL)
                );
                ShowWindow(enemyField[i], SW_SHOW);

                userFStartPos.X += g::CELL_W;
                enemyFStartPos.X += g::CELL_W;
            }

            ShowWindow(buttonStart, SW_HIDE);

            

        }

        if (LOWORD(wParam) >= ID_BUTTON_ENEMY_FIELD_BASE && LOWORD(wParam) < ID_BUTTON_ENEMY_FIELD_BASE + g::MAX_SIZE) {
            size_t cellId = LOWORD(wParam) - ID_BUTTON_ENEMY_FIELD_BASE;

            bool aiDoMove = false;
            bool restartGame = false;

            switch (enemyFieldStatus[cellId])
            {
            case FCStatus::shipPart_____:
                enemyFieldStatus[cellId] = FCStatus::destroyedPart;
                break;
            case FCStatus::empty________:
                aiDoMove = true;
                enemyFieldStatus[cellId] = FCStatus::noDestroyedPt;
                break;
            }

            SetWindowText(enemyField[cellId], getCellTextFromStatus(enemyFieldStatus[cellId], Team::enemy));

            if (!findAnyShip(enemyFieldStatus)) {
                MessageBox(hWnd, "YOU WIN!", "INFO", MB_OK | MB_ICONINFORMATION);
                restartGame = true;
                aiDoMove = false;
            }

            if (aiDoMove) {
                bool _do = true;
                size_t cellId;
                while (_do) {
                    cellId = rand() % g::MAX_SIZE;

                    switch (userFieldStatus[cellId])
                    {
                    case FCStatus::shipPart_____:
                        userFieldStatus[cellId] = FCStatus::destroyedPart;
                        _do = false;
                        break;
                    case FCStatus::empty________:
                        userFieldStatus[cellId] = FCStatus::noDestroyedPt;
                        _do = false;
                        break;
                    }

                }

                SetWindowText(userField[cellId], getCellTextFromStatus(userFieldStatus[cellId], Team::user));

                if (!findAnyShip(userFieldStatus)) {
                    MessageBox(hWnd, "YOU LOSE!", "INFO", MB_OK | MB_ICONINFORMATION);
                    restartGame = true;
                }

            }

            if (restartGame) {

                userFieldStatus.clear();
                enemyFieldStatus.clear();

                if (!userField.empty() && !enemyField.empty()) {
                    for (size_t i = 0; i < g::MAX_SIZE; ++i) {
                        DestroyWindow(userField[i]);
                        DestroyWindow(enemyField[i]);
                    }

                    userField.clear();
                    enemyField.clear();
                }

                ShowWindow(buttonStart, SW_SHOW);
            }
            

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