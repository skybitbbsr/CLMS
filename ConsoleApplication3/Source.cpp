
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>

int main(int argc, char **argv) {
	
	SQLHENV sqlhenv = SQL_NULL_HENV;
	//SQLHSTMT sqlhstmt = SQL_NULL_HSTMT;
	SQLHDBC sqldbc = SQL_NULL_HDBC;
	//SQLSMALLINT sql_conn_str_size = NULL;
	SQLWCHAR dsn_name [1024];
	SQLSMALLINT dsn_name_len;
	SQLSMALLINT direction = SQL_FETCH_FIRST;
	SQLWCHAR dsn_description[100];
	SQLWCHAR  sqlstate[SQL_SQLSTATE_SIZE + 1], message_text[100];
	SQLINTEGER	native_error;
	SQLSMALLINT message_text_length;
	SQLSMALLINT dsn_description_len;
	SQLRETURN ret;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlhenv);
	SQLSetEnvAttr(sqlhenv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3,NULL);
	SQLAllocHandle(SQL_HANDLE_DBC, sqlhenv, &sqldbc);

		//SQLAllocHandle(SQL_HANDLE_STMT, sqldbc, &sqlhstmt);

	//while (SQL_SUCCESS == (ret = SQLDataSources(sqlhenv, direction, dsn_name, 50, &dsn_name_len, dsn_description, 100, &dsn_description_len))) {
		//direction = SQL_FETCH_NEXT;
		//wprintf(L"%s - %s \n", dsn_name, dsn_description);
	//}

	
	/*if (ret == SQL_SUCCESS) {
		wprintf(L"SQL_SUCCESS");
	} 
	*/

	getchar();
	return 0;
 }