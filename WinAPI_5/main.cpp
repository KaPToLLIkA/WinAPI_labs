#include <Windows.h>
#include "utils.h"



namespace g {
    sql::Connection* connection = connect("tcp://127.0.0.1:3306", "root", "1k2o3s4t5y6A");
    sql::SQLString schema = "mydb";


    sql::SQLString requestStr;

    TABLE resultTable;
    std::string resultAnswer;
}


namespace appContext {



}



int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
) {
 
    g::connection->setSchema(g::schema);
    g::requestStr = sql::SQLString("SELECT * FROM x;");


    int code = executeRequest(g::connection, g::requestStr, g::resultAnswer, g::resultTable);
    if (code) {
        MessageBox(NULL, std::to_string(code).c_str(), "ERROR", MB_OK | MB_ICONERROR);
    }

    return EXIT_SUCCESS;
}