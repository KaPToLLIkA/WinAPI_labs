#include <windows.h>
#include <string>
#include <set>
#include <map>
#include <regex>
#include <fstream>
#include <vector>

#define ID_CLIENTWINDOW 24001
#define ID_WINDOWMENU 0
#define ID_MDIWINDOW 24002

#define CM_NEWMDIWINDOW 5

typedef int INT;



namespace g {
	HINSTANCE hInst;
	LPCSTR frameWindowName = "WINDOW";
	LPCSTR frameWindowClassName = "FRAMEWINDOW";
	LPCSTR frameWindowMenuName = "APPMENU";
	LPCSTR MDIwindowClassName = "DOC";
	HWND frameWindow;
	HWND clientWindow;
	HMENU frameWindowMenu;
	HMENU frameWindowMenuFile;

	std::string textFromFile;
	std::set<HWND> MDIWindows;
	std::map<HWND, HWND> textView;
	std::vector<std::string> textFromFileLines;
}


BOOL StartApp();

LRESULT CALLBACK WndFrameProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndMDIDocProc(HWND, UINT, WPARAM, LPARAM);

ATOM RegClass(LPCSTR, LPCSTR, WNDPROC, HINSTANCE, UINT, INT, INT);
VOID ShowLastError(LPCSTR);


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
	g::hInst = hInstance;

	if (hPrevInstance) return FALSE;
	if (!StartApp()) return FALSE;

	ShowWindow(g::frameWindow, SW_SHOW);
	UpdateWindow(g::frameWindow);

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
			if (!TranslateMDISysAccel(g::clientWindow, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}


	return msg.wParam;
}

BOOL StartApp()
{
	if (RegClass(
		g::frameWindowClassName,
		NULL,
		WndFrameProc,
		g::hInst,
		CS_HREDRAW | CS_VREDRAW,
		0,
		0) == 0)
	{
		ShowLastError("Can't register class.");
		return FALSE;
	}
	if (RegClass(
		g::MDIwindowClassName,
		NULL,
		WndMDIDocProc,
		g::hInst,
		CS_HREDRAW | CS_VREDRAW,
		0,
		0) == 0)
	{
		ShowLastError("Can't register class.");
		return FALSE;
	}

	g::frameWindow = CreateWindow(
		g::frameWindowClassName,
		g::frameWindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL,
		NULL,
		g::hInst,
		NULL
	);
	if (!g::frameWindow)
	{
		ShowLastError("Can't create frame Window.");
		return FALSE;
	}

	return TRUE;
}


// WNDPROC functions

LRESULT CALLBACK WndFrameProc(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
) {

	static HWND button_user_error_create;
	switch (Msg) {

	case WM_CREATE:
	{
		g::frameWindowMenu = CreateMenu();

		SetMenu(hWnd, g::frameWindowMenu);

		g::frameWindowMenuFile = CreatePopupMenu();
		AppendMenu(g::frameWindowMenuFile, MF_ENABLED | MF_STRING,
			CM_NEWMDIWINDOW, "New MDI window");


		AppendMenu(g::frameWindowMenu, MF_ENABLED | MF_POPUP,
			(UINT)g::frameWindowMenuFile, "File");

		DrawMenuBar(hWnd);

		CLIENTCREATESTRUCT clcs;
		clcs.hWindowMenu = GetSubMenu(GetMenu(hWnd), ID_WINDOWMENU);
		clcs.idFirstChild = ID_MDIWINDOW;

		g::clientWindow = CreateWindow(
			"MDICLIENT",
			NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE |
			WS_HSCROLL | WS_VSCROLL,
			0, 0, 0, 0,
			hWnd,
			(HMENU)ID_CLIENTWINDOW,
			g::hInst,
			(LPSTR)&clcs);

		if (!g::clientWindow)
		{
			ShowLastError("Can't create client Window.");
			return FALSE;
		}


	}
	break;

	case WM_COMMAND:
	{

		switch (wParam) {
		case CM_NEWMDIWINDOW:
		{
			MDICREATESTRUCT    mdics;

			mdics.szClass = g::MDIwindowClassName;
			mdics.szTitle = "MDI Child Window";
			mdics.hOwner = g::hInst;
			mdics.x = CW_USEDEFAULT;
			mdics.y = CW_USEDEFAULT;
			mdics.cx = CW_USEDEFAULT;
			mdics.cy = CW_USEDEFAULT;
			mdics.style = 0;
			mdics.lParam = NULL;

			g::MDIWindows.insert((HWND)SendMessage(g::clientWindow,
				WM_MDICREATE, 0, (LPARAM)&mdics));

		}
		break;


		}
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	break;
	}


	return DefFrameProc(hWnd, g::clientWindow, Msg, wParam, lParam);
}

LRESULT CALLBACK WndMDIDocProc(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
) {
	static HWND buttonOpen;
	static HWND fileNameInput;


	switch (Msg) {
	case WM_PAINT:
	{
		if (!g::textFromFile.empty()) {
			int h = 30;
			int deltah = 20;

			PAINTSTRUCT ps;
			HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			HDC hdc = BeginPaint(hWnd, &ps);
			for (auto line : g::textFromFileLines) {

				TextOut(hdc, 310, h, line.c_str(), line.size());
				h += deltah;
			}
			EndPaint(hWnd, &ps);
		}

	}
	break;
	case WM_CREATE:
	{

		if (g::MDIWindows.size() == 0) {

			MessageBeep(MB_ICONWARNING);

			fileNameInput = CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
				5, 5, 300, 20, hWnd, NULL, g::hInst, 0);
			ShowWindow(fileNameInput, SW_NORMAL);

			buttonOpen = CreateWindow("Button", "Open file", WS_CHILD | WS_VISIBLE | WS_BORDER,
				310, 5, 80, 20, hWnd, NULL, g::hInst, 0);
			ShowWindow(buttonOpen, SW_NORMAL);

		}

		HWND text = CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY,
			5, 30, 300, 200, hWnd, NULL, g::hInst, 0);
		ShowWindow(text, SW_NORMAL);


		if (g::textFromFile.empty())
			SetWindowText(text, "OPEN FILE");
		else
			SetWindowText(text, g::textFromFile.c_str());


		g::textView.insert(std::make_pair(hWnd, text));
	}
	break;
	case WM_DESTROY:
	{
		g::MDIWindows.erase(hWnd);
		g::textView.erase(hWnd);

	}
	break;
	case WM_COMMAND:
	{
		if (buttonOpen == (HWND)lParam) {
			LPSTR buffer = new CHAR[1024];
			memset(buffer, '\0', 1024);
			GetWindowText(fileNameInput, buffer, 1024);

			std::regex filter("\\.txt$");

			if (std::regex_search(std::string(buffer), filter)) {
				std::ifstream infile(buffer);
				std::string tmp;
				g::textFromFile.clear();
				g::textFromFileLines.clear();

				while (!infile.eof()) {
					std::getline(infile, tmp, '\n');
					g::textFromFile.append(tmp);
					g::textFromFileLines.push_back(tmp);
				}

				infile.close();


				for (auto handle : g::textView)
					SetWindowText(handle.second, g::textFromFile.c_str());


				for (auto handle : g::MDIWindows)
					InvalidateRect(handle, NULL, TRUE);
			}
			else {
				MessageBox(hWnd, "ONLY TXT FILES", "ERROR", MB_OK | MB_ICONERROR);
			}

			delete[] buffer;
		}

	}
	break;

	}


	return DefMDIChildProc(hWnd, Msg, wParam, lParam);
}


// functions without external dependences
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
