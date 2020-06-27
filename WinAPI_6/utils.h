#pragma comment (lib, "comctl32")

#include <Windows.h>
#include <CommCtrl.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "jdbc/mysql_connection.h"

#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>






#define RCX(rc) (rc.left)
#define RCY(rc) (rc.top)
#define RCW(rc) (rc.right - rc.left)
#define RCH(rc) (rc.bottom - rc.top)


typedef std::vector<std::string> ROW;
typedef std::vector<ROW> TABLE;




sql::Connection* connect(sql::SQLString address, sql::SQLString user, sql::SQLString password);


int executeRequest(sql::Connection* con, sql::SQLString& request, std::string& answer, TABLE& table);


HWND createTable(int id, HWND &hwndParent, HINSTANCE &hInst, RECT rc);

HWND createButton(
	std::string text, 
	int id, 
	HWND& hwndParent, 
	HINSTANCE& hInst, 
	RECT rc
);

HWND createComboBox(
	std::string text,
	int id,
	HWND& hwndParent,
	HINSTANCE& hInst,
	RECT rc
);

void insertRowsInComboBox(
	std::vector<std::string> rows,
	HWND &comboBox
);

void clearComboBox(
	HWND& comboBox
);