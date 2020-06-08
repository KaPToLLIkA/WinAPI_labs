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
