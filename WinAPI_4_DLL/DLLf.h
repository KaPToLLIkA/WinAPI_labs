#pragma once
#include <Windows.h>



#ifdef DLLLAB_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#define	T_LINE		0
#define T_ELLIPSE	1
#define T_RECTANGLE	2

typedef struct PEN_DESCRIPTOR {
	int fnPenStyle = PS_SOLID, nWidth = 1;
	COLORREF color = RGB(0, 0, 0);
} PEN_DESC;


typedef struct FIGURE_DESCRIPTOR {
	RECT rect = { 0, 0, 0, 0 };
	PEN_DESC pen;
	LONG type = T_LINE;
} FIGURE_DESC;


typedef FIGURE_DESC* DRAWABLEITEMS;


extern "C" DLL_API VOID DrawScene(
	HDC hdc,
	DRAWABLEITEMS  items,
	LONG size);

extern "C" DLL_API VOID DrawItem(
	HDC hdc,
	const FIGURE_DESC & item);


//DRAWABLEITEMS& GenerateCirclesGrid(HWND, DRAWABLEITEMS&);
//
//DRAWABLEITEMS& GenerateRectanglesGrid(HWND, DRAWABLEITEMS&);
