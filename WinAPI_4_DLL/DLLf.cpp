#include "pch.h"
#include "DLLf.h"

VOID DrawScene(
	HDC hdc, 
	DRAWABLEITEMS items,
	LONG size) {
	
	HPEN pen, oldPen;
	

	for (LONG i = 0; i < size; ++i) {
		
		pen = CreatePen(
			items[i].pen.fnPenStyle,
			items[i].pen.nWidth,
			items[i].pen.color);
		

		oldPen = (HPEN)SelectObject(hdc, (HPEN)pen);
		DeleteObject(oldPen);

		switch (items[i].type) {
		case T_LINE:
		{
			MoveToEx(
				hdc,
				items[i].rect.left,
				items[i].rect.top,
				NULL);
			LineTo(
				hdc,
				items[i].rect.right,
				items[i].rect.bottom);

		}
		break;
		case T_ELLIPSE:
		{
			Ellipse(
				hdc,
				items[i].rect.left,
				items[i].rect.top,
				items[i].rect.right,
				items[i].rect.bottom);

		}
		break;
		case T_RECTANGLE:
		{
			Rectangle(
				hdc,
				items[i].rect.left,
				items[i].rect.top,
				items[i].rect.right,
				items[i].rect.bottom);
		}
		break;
		}

	}
	
}


VOID DrawItem(
	HDC hdc,
	const FIGURE_DESC& item) {

	

	HPEN pen, oldPen;

	pen = CreatePen(
		item.pen.fnPenStyle,
		item.pen.nWidth,
		item.pen.color);

	oldPen = (HPEN)SelectObject(hdc, (HPEN)pen);
	DeleteObject(oldPen);
	switch (item.type) {
	case T_LINE:
	{
		MoveToEx(
			hdc,
			item.rect.left,
			item.rect.top,
			NULL);
		LineTo(
			hdc,
			item.rect.right,
			item.rect.bottom);

	}
	break;
	case T_ELLIPSE:
	{
		Ellipse(
			hdc,
			item.rect.left,
			item.rect.top,
			item.rect.right,
			item.rect.bottom);

	}
	break;
	case T_RECTANGLE:
	{
		Rectangle(
			hdc,
			item.rect.left,
			item.rect.top,
			item.rect.right,
			item.rect.bottom);
	}
	break;
	}
	
}


//DRAWABLEITEMS& GenerateCirclesGrid(HWND hWnd, DRAWABLEITEMS& items)
//{
//	RECT clientRect;
//	GetClientRect(hWnd, &clientRect);
//
//	items.clear();
//
//
//
//
//	return items;
//}
//
//DRAWABLEITEMS& GenerateRectanglesGrid(HWND hWnd, DRAWABLEITEMS& items)
//{
//	RECT clientRect;
//	GetClientRect(hWnd, &clientRect);
//
//	items.clear();
//
//
//
//
//	return items;
//}
