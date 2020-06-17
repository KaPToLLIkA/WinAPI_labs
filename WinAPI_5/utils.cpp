#include "utils.h"


sql::Connection* connect(sql::SQLString address, sql::SQLString user, sql::SQLString password)
{
	return get_driver_instance()->connect(address, user, password);
}

int executeRequest(sql::Connection* con, sql::SQLString& request, std::string& answer, TABLE& table)
{
	answer.clear();
	table.clear();
	
	sql::Statement* stmt = con->createStatement();
	sql::ResultSet* res;
	try {
		res = stmt->executeQuery(request);

	}
	catch (sql::SQLException ex) {
		return ex.getErrorCode();
	}

	uint32_t columnCount = res->getMetaData()->getColumnCount();

	while (res->next()) {
		ROW newRow;
		for (uint32_t i = 1; i <= columnCount; ++i) {
			answer += std::string(res->getString(i).c_str()) + "[SEPARATOR]";
			newRow.push_back(std::string(res->getString(i).c_str()));
		}
		answer += "\n";
		table.push_back(newRow);
	}


	return 0;
}

HWND createTable(int id, HWND& hwndParent, HINSTANCE& hInst, RECT rc)
{
	
	INITCOMMONCONTROLSEX icex;          
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	

	HWND hWndListView = CreateWindow(WC_LISTVIEW,
		"",
		WS_CHILD | LVS_REPORT | LVS_EDITLABELS | WS_VISIBLE,
		rc.left, rc.top,
		rc.right - rc.left,
		rc.bottom - rc.top,
		hwndParent,
		(HMENU)id,
		hInst,
		NULL);

	

	return (hWndListView);

}

HWND createButton(
	std::string text, 
	int id, 
	HWND& hwndParent, 
	HINSTANCE& hInst, 
	RECT rc)
{
	return CreateWindow(
	"BUTTON",
		text.c_str(),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		RCX(rc),
		RCY(rc),
		RCW(rc),
		RCH(rc),
		hwndParent,
		(HMENU)id,
		hInst,
		NULL
	);
}

HWND createComboBox(
	std::string text, 
	int id, 
	HWND& hwndParent, 
	HINSTANCE& hInst, 
	RECT rc)
{
	return CreateWindow(
		"COMBOBOX",
		text.c_str(),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_SIMPLE,
		RCX(rc), RCY(rc),
		RCW(rc), RCH(rc),
		hwndParent,
		(HMENU)id,
		hInst,
		NULL
	);
}

void insertRowsInComboBox(std::vector<std::string> rows, HWND& comboBox)
{
	TCHAR tmp[1024];

	for (int i = 0; i < rows.size(); ++i) {
		memset(tmp, '\0', 1024);
		memcpy(tmp, rows[i].c_str(), rows[i].size());
		SendMessage(comboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);
	}

	

}

void clearComboBox(HWND& comboBox)
{
	int count = SendMessage(comboBox, (UINT)CB_GETCOUNT, (WPARAM)0, (LPARAM)0);

	for (int i = 0; i < count; ++i) {

		SendMessage(comboBox, (UINT)CB_DELETESTRING, (WPARAM)0, (LPARAM)0);
	}
}




