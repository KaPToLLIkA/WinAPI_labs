#include <windows.h>
#include <string>
#include "resource.h"
#include "DLLf.h"

#define CM_SELECT_LINE		1
#define CM_SELECT_RECTANGLE 2
#define CM_SELECT_CIRCLE	3

#define CM_SELECT_COLOR		4



typedef int INT;


namespace g {
	//scene

	PAINTSTRUCT pStr;
	HDC hdc;
	BOOL IsStartedDrawingNewElement = FALSE;
	FIGURE_DESC curFigure;

	DRAWABLEITEMS allItems = new FIGURE_DESC[1024];
	LONG maxi = 0;
	WNDPROC oldEditProc;



	//window
	HINSTANCE hInst, dllInst;
	HWND Window;
	LPCSTR WindowName = "WINDOW";
	LPCSTR WindowClassName = "WINDOW";

	HMENU WindowMainMenu;
	HMENU WindowMainMenuFile;

	//pen
	TCHAR penStyleNames[7][12] = {
		"SOLID",
		"DASH",
		"DOT",
		"DASHDOT",
		"DASHDOTDOT",
		"NULL",
		"INSIDEFRAME"
	};

	LONG maxPenWidth = 25;

}

typedef VOID(*DrawScene)(HDC, DRAWABLEITEMS, LONG);
typedef VOID(*DrawItem)(HDC, const FIGURE_DESC&);
DrawScene scene;
DrawItem item;

BOOL StartApp();

BOOL CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditProc(HWND, UINT, WPARAM, LPARAM);

ATOM RegClass(LPCSTR, LPCSTR, WNDPROC, HINSTANCE, UINT, INT, INT);
VOID ShowLastError(LPCSTR);
UINT64 GetUINTFromEdit(HWND, UINT64);


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
	g::dllInst = LoadLibrary("D:\\Development\\Projects\\C++\\repos\\DllLab\\Release\\DLLlab.dll");
	if (!g::dllInst) {
		ShowLastError("LoadLibrary Error!");
		return FALSE;
	}

	scene = (DrawScene)GetProcAddress((HMODULE)g::dllInst, "DrawScene");
	item = (DrawItem)GetProcAddress((HMODULE)g::dllInst, "DrawItem");


	g::hInst = hInstance;

	if (hPrevInstance) return FALSE;
	if (!StartApp()) return FALSE;

	ShowWindow(g::Window, SW_SHOW);
	UpdateWindow(g::Window);

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

	FreeLibrary(g::dllInst);
	return msg.wParam;
}

BOOL StartApp()
{
	if (RegClass(
		g::WindowClassName,
		NULL,
		WndProc,
		g::hInst,
		CS_HREDRAW | CS_VREDRAW,
		0,
		0) == 0)
	{
		ShowLastError("Can't register class.");
		return FALSE;
	}


	g::Window = CreateWindow(
		g::WindowClassName,
		g::WindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL,
		NULL,
		g::hInst,
		NULL
	);
	if (!g::Window)
	{
		ShowLastError("Can't create frame Window.");
		return FALSE;
	}

	return TRUE;
}


// WNDPROC functions

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
) {



	switch (Msg) {
	case WM_CREATE:
	{
		g::WindowMainMenu = CreateMenu();

		SetMenu(hWnd, g::WindowMainMenu);

		g::WindowMainMenuFile = CreatePopupMenu();
		AppendMenu(g::WindowMainMenuFile, MF_ENABLED | MF_STRING,
			CM_SELECT_LINE, "LINE");

		AppendMenu(g::WindowMainMenuFile, MF_ENABLED | MF_STRING,
			CM_SELECT_RECTANGLE, "RECTANGLE");

		AppendMenu(g::WindowMainMenuFile, MF_ENABLED | MF_STRING,
			CM_SELECT_CIRCLE, "CIRCLE");

		AppendMenu(g::WindowMainMenuFile, MF_ENABLED | MF_STRING,
			CM_SELECT_COLOR, "COLOR");


		AppendMenu(g::WindowMainMenu, MF_ENABLED | MF_POPUP,
			(UINT)g::WindowMainMenuFile, "Select...");

		DrawMenuBar(hWnd);

	}
	break;
	case WM_PAINT:
	{
		g::hdc = BeginPaint(hWnd, &g::pStr);

		scene(g::hdc, g::allItems, g::maxi);

		if (g::IsStartedDrawingNewElement)
			item(g::hdc, g::curFigure);


		EndPaint(hWnd, &g::pStr);
	}
	break;
	case WM_COMMAND:
	{
		switch (wParam) {
		case CM_SELECT_LINE:
		{
			MessageBeep(MB_ICONINFORMATION);
			g::curFigure.type = T_LINE;
		}
		break;
		case CM_SELECT_RECTANGLE:
		{
			MessageBeep(MB_ICONINFORMATION);
			g::curFigure.type = T_RECTANGLE;
		}
		break;
		case CM_SELECT_CIRCLE:
		{
			MessageBeep(MB_ICONINFORMATION);
			g::curFigure.type = T_ELLIPSE;
		}
		break;
		case CM_SELECT_COLOR:
		{
			MessageBeep(MB_ICONINFORMATION);
			DialogBox(g::hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
		}
		break;
		}

	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (!g::IsStartedDrawingNewElement) {
			g::curFigure.rect.left = LOWORD(lParam);
			g::curFigure.rect.top = HIWORD(lParam);
			g::IsStartedDrawingNewElement = TRUE;
		}
		else
		{

			g::IsStartedDrawingNewElement = FALSE;
			g::allItems[g::maxi++] = g::curFigure;
		}
		InvalidateRect(hWnd, NULL, TRUE);
	}
	case WM_MOUSEMOVE:
	{
		g::curFigure.rect.right = LOWORD(lParam);
		g::curFigure.rect.bottom = HIWORD(lParam);
		if (g::IsStartedDrawingNewElement) InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
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





BOOL CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND redit, gedit, bedit, combobox, wedit;
	static PAINTSTRUCT pStr;
	static HDC hdc;
	static RECT clientRect;
	static HBRUSH hbr, oldhbr;
	static UINT8 r{ 0 }, g{ 0 }, b{ 0 };
	static LONG curPenStyle = 0;

	switch (message) {
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &pStr);
		r = (UINT8)GetUINTFromEdit(redit, 256);
		g = (UINT8)GetUINTFromEdit(gedit, 256);
		b = (UINT8)GetUINTFromEdit(bedit, 256);

		hbr = CreateSolidBrush(RGB(r, g, b));
		oldhbr = (HBRUSH)SelectObject(hdc, (HGDIOBJ)hbr);
		Rectangle(hdc, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
		hbr = (HBRUSH)SelectObject(hdc, (HGDIOBJ)oldhbr);
		DeleteObject(hbr);
		EndPaint(hWnd, &pStr);
	}
	break;
	case WM_INITDIALOG:
	{
		redit = GetDlgItem(hWnd, IDC_R);
		gedit = GetDlgItem(hWnd, IDC_G);
		bedit = GetDlgItem(hWnd, IDC_B);
		wedit = GetDlgItem(hWnd, IDC_W);

		SetWindowText(redit, "0");
		SetWindowText(gedit, "0");
		SetWindowText(bedit, "0");
		SetWindowText(wedit, "1");

		g::oldEditProc = (WNDPROC)SetWindowLongPtr(redit, GWLP_WNDPROC, (LONG_PTR)EditProc);
		SetWindowLongPtr(gedit, GWLP_WNDPROC, (LONG_PTR)EditProc);
		SetWindowLongPtr(bedit, GWLP_WNDPROC, (LONG_PTR)EditProc);
		SetWindowLongPtr(wedit, GWLP_WNDPROC, (LONG_PTR)EditProc);


		combobox = GetDlgItem(hWnd, IDC_COMBO);


		TCHAR tmp[12];

		for (int i = 0; i < 7; ++i) {
			memset(tmp, 0, 12);
			memcpy(tmp, g::penStyleNames[i], 12);
			SendMessage(combobox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);
		}
		GetClientRect(hWnd, &clientRect);
	}
	break;
	case WM_COMMAND:
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			MessageBeep(MB_ICONINFORMATION);
			curPenStyle = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
				(WPARAM)0, (LPARAM)0);

		}
		switch (LOWORD(wParam)) {
		case IDCANCEL:
		{
			EndDialog(hWnd, 0);
			return TRUE;
		}
		break;
		case IDOK:
		{


			g::curFigure.pen.color = RGB(r, g, b);
			g::curFigure.pen.fnPenStyle = curPenStyle;
			g::curFigure.pen.nWidth = GetUINTFromEdit(wedit, g::maxPenWidth);

			EndDialog(hWnd, 0);
			return TRUE;
		}
		break;
		}
	}
	case WM_KEYDOWN:
	{
		InvalidateRect(hWnd, NULL, TRUE);
	}
	break;
	default: return FALSE;
	}
}




LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CHAR)
	{
		if (!((wParam >= '0' && wParam <= '9')
			|| wParam == '.'
			|| wParam == VK_RETURN
			|| wParam == VK_DELETE
			|| wParam == VK_BACK))
			return 0;
	}
	return g::oldEditProc(hWnd, uMsg, wParam, lParam);
};


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

UINT64 GetUINTFromEdit(HWND edit, UINT64 maxVal) {

	static char buffer[1024];

	memset(buffer, '\0', 1024);
	GetWindowText(edit, buffer, 1024);
	try {
		return std::stoul(std::string(buffer)) % maxVal;
	}
	catch (...) {
		return 0;
	}
}



