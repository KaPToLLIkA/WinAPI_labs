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


#define ID_TABLE 10001


typedef std::vector<std::string> ROW;
typedef std::vector<ROW> TABLE;




sql::Connection* connect(sql::SQLString address, sql::SQLString user, sql::SQLString password);


int executeRequest(sql::Connection* con, sql::SQLString& request, std::string& answer, TABLE& table);


HWND createTable(HWND &hwndParent, HINSTANCE &hInst, RECT rc);
