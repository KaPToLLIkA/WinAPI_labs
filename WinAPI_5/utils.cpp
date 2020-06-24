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
		answer = ex.getSQLStateCStr();
		return ex.getErrorCode();
	}

	uint32_t columnCount = res->getMetaData()->getColumnCount();
	
	ROW columns;
	for (uint32_t i = 1; i <= columnCount; ++i) {

		columns.push_back(std::string(res->getMetaData()->getColumnLabel(i).c_str()));
		
	}
	table.push_back(columns);

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
	//InitCommonControls();
	

	HWND hWndListView = CreateWindow(WC_LISTVIEW, "",
		WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | LVS_EX_FULLROWSELECT,
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




