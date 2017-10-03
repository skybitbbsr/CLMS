
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdio.h>


int main(int argc, char **argv) {
	
	SQLHENV env;
	SQLWCHAR driver[256];
	SQLWCHAR attr[256];
	SQLSMALLINT driver_ret;
	SQLSMALLINT attr_ret;
	SQLUSMALLINT direction;
	SQLRETURN ret;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

	direction = SQL_FETCH_FIRST;
	while (ret = SQLDriversW(env, direction,
		driver, sizeof(driver), &driver_ret,
		attr, sizeof(attr), &attr_ret)) {
		direction = SQL_FETCH_NEXT;
		printf("%s - %s\n", driver, attr);
		if (ret == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
	}
	wprintf(driver);
	getchar();
	return 0;
 }