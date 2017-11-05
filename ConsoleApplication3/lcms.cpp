#include "lcms.h"



SQLHENV henv_sql;
SQLHDBC hdbc_sql;
SQLHSTMT hstmt_sql;
SQLRETURN ret_sql;
SQLWCHAR* connectionString;


//Init function -- This function is the function which is gonna be called to allocate the handles.
void INITHandles(SQLCHAR* ConnectionString) {
	printf("Allocating environment handle:");
	ret_sql = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv_sql);
	if (ret_sql == SQL_SUCCESS)
		printf("Success\n");
	else {
		printf("Unable to allocate handle!!");
		exit(-1);
	}

	//Setting ODBC 3.0 functionality
	SQLSetEnvAttr(henv_sql, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	printf("Allocating connection handle: ");
	ret_sql = SQLAllocHandle(SQL_HANDLE_DBC, henv_sql, &hdbc_sql);
	if (ret_sql == SQL_SUCCESS)
		printf("Success\n");
	else {
		show_error("sql handle connection", SQL_HANDLE_ENV, &henv_sql);
		exit(-1);
	}

	//After allocation of the connection handle we try connect to the Data Source using the provided connection string.
	printf("Connecting to the Data Source: ");
	ret_sql = SQLDriverConnectA(hdbc_sql, NULL, ConnectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
	if (ret_sql == SQL_SUCCESS)
		printf("Success\n");
	else {
		show_error("try to connect to data source", SQL_HANDLE_DBC, hdbc_sql);
		exit(-2);
	}

	//Then allocating sttement handle for future sql execution
	ret_sql = SQLAllocHandle(SQL_HANDLE_STMT, hdbc_sql, &hstmt_sql);
	if (ret_sql == SQL_SUCCESS)
		printf("Success\n");
	else {
		show_error("try to connect to data source", SQL_HANDLE_DBC, hdbc_sql);
		exit(-2);
	}
}


//Function which is going to be used frequently
void gotoxy(short col, short row)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { col,row };
	SetConsoleCursorPosition(h, position);
}


//This functions is for extractig error string simply said "The reason of occuring this error"
void show_error(char* purp, SQLSMALLINT type, SQLHANDLE handle) {

	SQLSMALLINT i = 0;
	SQLINTEGER native;
	SQLWCHAR state[7];
	SQLWCHAR text[256];
	SQLSMALLINT size;

	fprintf(stderr, "Error have been occured after this operation : %s", purp);

	while (SQLGetDiagRec(type, handle, ++i, state, &native, text, sizeof(text), &size) == SQL_SUCCESS) {
		fwprintf(stderr, L"%s:%ld:%ld:%s\n", state, i, native, text);
	}

}


//Single function for lodging complaints in the complaints
void lodge_complaints() {

	//required data for complaint table
	SQLCHAR* problemType = (SQLCHAR*)malloc(sizeof(SQLCHAR) * 20);
	SQLCHAR* description = (SQLCHAR*)malloc(sizeof(SQLCHAR) * 45);
	SQLINTEGER student_id, machine_id;
	SQLLEN problem_len = 20, description_len = 45;
	SQLCHAR* syntax = (SQLCHAR*)"INSERT INTO lodged_issue (stuId,machineId,problemType,description) VALUES (?,?,?,?) ";


//Getting all the inputs from the users
	printf("Enter Studnet ID: ");
	scanf_s("%d", &student_id);
	printf("Enter Machine ID: ");
	scanf_s("%d", &machine_id);
	printf("Enter ProblemType: ");
	scanf_s("%s", problemType,44);
	printf("Enter description: ");
	scanf_s("%[^\n]", description,44);


	//Preparing sql syntax to execute
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);
	
	if (ret_sql != SQL_SUCCESS) {
		show_error("Preparing sql state", SQL_HANDLE_STMT, hstmt_sql);
		
		exit(-1);
	}

	//After preparing the statemant next work is to bind the param
		ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 3, 0, &student_id, 0, NULL);

		if (ret_sql != SQL_SUCCESS) {
			show_error("binding student_id", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}
		ret_sql = SQLBindParameter(hstmt_sql, 2, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &machine_id, 0, NULL);
		
		if (ret_sql != SQL_SUCCESS) {
			show_error("binding machine_id", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}
		ret_sql = SQLBindParameter(hstmt_sql,3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0,problemType,problem_len,NULL);
		
		if (ret_sql != SQL_SUCCESS) {
			show_error("binding problemtype", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}
			ret_sql = SQLBindParameter(hstmt_sql, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 45, 0,description, description_len,NULL);

			if (ret_sql != SQL_SUCCESS) {
				show_error("binding description", SQL_HANDLE_STMT, hstmt_sql);
				exit(-1);
			}

//Executing prepared sql statements


		ret_sql = SQLExecute(hstmt_sql);

		if (ret_sql != SQL_SUCCESS) {
			show_error("inserting data to complaint", SQL_HANDLE_STMT, hstmt_sql);
			exit(-2);
		}

	printf("Successfully lodged.");
}

//Fuction for gettind data from user about a student
void add_student() {

	//Rrequired data from user about student
	SQLCHAR* firstname = (SQLCHAR*)malloc(sizeof(SQLCHAR) * 20);
	SQLCHAR* lastname = (SQLCHAR*)malloc(sizeof(SQLCHAR) * 20);
	SQLCHAR* middlename = (SQLCHAR*)malloc(sizeof(SQLCHAR*) * 20);
	SQLCHAR* registrationNumber = (SQLCHAR*)malloc(sizeof(SQLCHAR) * 20);
	SQLINTEGER semester;
	SQLCHAR syntax[] = "INSERT INTO STUDENT(firstName,lastName,middleName,registrationNumber,semester) VALUES (?,?,?,?,?)";


	//Getting data 
	printf("Enter student first name: ");
	scanf_s("%s",firstname,20);
	printf("\nEnter student middle name: ");
	scanf_s("%s",middlename,20);
	printf("\nEnter student last name: ");
	scanf_s("%s", lastname,20);
	printf("\nEnter student registration : ");
	scanf_s("%s", registrationNumber,20);
	printf("\nEnter student semester: ");
	scanf_s("%d", &semester);




	
	//preparing sql syntax
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);
	if (ret_sql != SQL_SUCCESS) {
		show_error("studnet sql syntax", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//Setting input parameter binding
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, firstname, 20, NULL);
	
	if (ret_sql != SQL_SUCCESS) {
		show_error("binding fn paramter student", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

		ret_sql = SQLBindParameter(hstmt_sql, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, lastname, 20, NULL);

		if (ret_sql != SQL_SUCCESS) {
			show_error("binding ln paramter student", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}

		ret_sql = SQLBindParameter(hstmt_sql, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, middlename, 20, NULL);

		if (ret_sql != SQL_SUCCESS) {
			show_error("binding mn paramter student", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}

		ret_sql = SQLBindParameter(hstmt_sql, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, registrationNumber, 20, NULL);

		if (ret_sql != SQL_SUCCESS) {
			show_error("binding rn paramter student", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}

		
		ret_sql = SQLBindParameter(hstmt_sql, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,11, 0, &semester, 11, NULL);

		if (ret_sql != SQL_SUCCESS) {
			show_error("binding rn paramter student", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}

		
			ret_sql = SQLExecute(hstmt_sql);

			if (ret_sql != SQL_SUCCESS) {
				show_error("inserting data to complaint", SQL_HANDLE_STMT, hstmt_sql);
				exit(-2);
			}
			
		printf("Successfully inserted!");
			
	}

//This is for inserting admins
void add_admin() {

	//required data to insert
	SQLINTEGER  adminId;
	SQLCHAR* name = (SQLCHAR*)malloc(sizeof(SQLCHAR*) * 40);
	SQLCHAR* syntax = (SQLCHAR*)"INSERT INTO admin values (?,?)";

	//Getting data
	printf("Enter admin ID: ");
	scanf_s("%d", &adminId);

	printf("Enter admin name:");
	scanf_s("%s", name,40);

	//preparing sql statement
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);

	if (ret_sql != SQL_SUCCESS) {
		show_error("preparing admin sql syntax", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//binding the parameter
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &adminId, 0, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding admin id", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	ret_sql = SQLBindParameter(hstmt_sql, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 40, 0, name,40 , NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding admin name", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//Executing sql syntax
		ret_sql = SQLExecute(hstmt_sql);

		if (ret_sql != SQL_SUCCESS) {
			show_error("Error in executing sql", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}

	printf("Successfully Executed!");
}

//This functions is for releasing all the allocated handles before terminating the program
void free_handle()
{

	//dealocating statement handle
	ret_sql = SQLFreeHandle(SQL_HANDLE_STMT, hstmt_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("freeing statement handle", SQL_HANDLE_DBC, hdbc_sql);
		exit(-1);
	}

	printf("Successfully freed statement hadnle!\n");



	//disconnecting connection handle
	ret_sql = SQLDisconnect(hdbc_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("terminating driver connection", SQL_HANDLE_DBC, hdbc_sql);
		exit(-1);
	}
	printf("Successfully disconnected!\n");

	//dealocating connection handle
	ret_sql = SQLFreeHandle(SQL_HANDLE_DBC, hdbc_sql);
	
	if (ret_sql != SQL_SUCCESS) {
		show_error("freeing coneection handle", SQL_HANDLE_DBC, hdbc_sql);
		exit(-1);
	}

	printf("Successfully freed connection hadnle!\n");

	//dealocating connection handle
	ret_sql = SQLFreeHandle(SQL_HANDLE_ENV, henv_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("freeing environment handle", SQL_HANDLE_DBC, hdbc_sql);
		exit(-1);
	}

	printf("Successfully freed environment hadnle!\n");
}

//this methdo required 
void add_lab()
{
	//Required data for lab
	SQLINTEGER labId;
	SQLCHAR* dept = (SQLCHAR*)malloc(sizeof(SQLCHAR) * 10);
	SQLCHAR* syntax = (SQLCHAR*)"INSERT INTO lab values(?,?)";

	//getting data
	printf("Enter labId: ");
	scanf_s("%d", &labId);
	printf("Enter dept: ");
	scanf_s("%s", dept,10);

	//preparing sql syntax
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);
	if (ret_sql != SQL_SUCCESS) {
		show_error("preparing sql querry lab", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//binding paramters
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 10, 0, &labId,
		10, NULL);
	
	if (ret_sql != SQL_SUCCESS) {
		show_error("binding labid", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}
	
	ret_sql = SQLBindParameter(hstmt_sql, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_C_CHAR, 10, 0, &dept,
		10, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding dept", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//Executing stament
	
		ret_sql = SQLExecute(hstmt_sql);
		if (ret_sql != SQL_SUCCESS) {
			show_error("inserting lab data", SQL_HANDLE_STMT, hstmt_sql);
			exit(-1);
		}

	printf("Successfully Executed!");
}


void add_machine()
{

	//reqruired data
	SQLINTEGER labId, machineId;
	SQLCHAR* syntax = (SQLCHAR*) "INSERT INTO machine VALUES (?,?)";

	//getting data
	printf("Enter labId to which that computer belongs: ");
	scanf_s("%d", &labId);
	printf("Enter the computerId: ");
	scanf_s("%d", &machineId);

	//preparing sql syntax
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);
	if (ret_sql != SQL_SUCCESS) {
		show_error("preparing syntax for machine", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}


	// binding sql paramter
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &machineId,
		11, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding machineId for machine", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	ret_sql = SQLBindParameter(hstmt_sql, 2, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &labId,
		11, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding labId for machine", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}


	//executing syntax
	ret_sql = SQLExecute(hstmt_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("Executing syntax for machine", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	printf("Successfully executed!");

}


//This function is for admins eg- when an admin finishes solving error then he usses this to track solved errors
void resolved_issues()
{
	//requierd data
	SQLINTEGER issueId , adminId;
	SQLCHAR* dateOfSolve = (SQLCHAR*)malloc(sizeof(SQLCHAR) * 15);
	SQLCHAR* syntax = (SQLCHAR*)"INSERT INTO resolved_issue values (?,?,?)";


	//getting user data
	printf("Enter issueId: ");
	scanf_s("%d", &issueId);
	printf("Enter the adminId who solved the issue: ");
	scanf_s("%d", &adminId);
    printf("Enter the date in the following format (YY-MM-DD) : ");
	scanf_s("%s", dateOfSolve,12);


	//preparing sql statement
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);

	if (ret_sql != SQL_SUCCESS) {
		show_error("preparing synatx for resolved issue", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//binding paramters
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &issueId, 11, NULL);
	
	if (ret_sql != SQL_SUCCESS) {
		show_error("Binding paramter issueId for resolved_issue", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	ret_sql = SQLBindParameter(hstmt_sql, 2, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &adminId, 11, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("Binding paramter adminId for resolved_issue", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}
	
	ret_sql = SQLBindParameter(hstmt_sql, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_DATE, SQL_DATE_LEN, 0, dateOfSolve, SQL_DATE_LEN, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("Binding paramter issueId for resolved_issue", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}


	//executing syntax
	ret_sql = SQLExecute(hstmt_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("Executing syntax for relosved_issue", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	printf("Successfully executed!");
}

void del_lodge_complaints()
{


	//required data for complaint table
	SQLINTEGER issueId;
	SQLCHAR* syntax = (SQLCHAR*)"DELETE FROM lodged_issue WHERE issueId = ?";


	//Getting all the inputs from the users
	printf("Enter Issue ID: ");
	scanf_s("%d", &issueId);


	//Preparing sql syntax to execute
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);

	if (ret_sql != SQL_SUCCESS) {
		show_error("Preparing sql statement", SQL_HANDLE_STMT, hstmt_sql);

		exit(-1);
	}

	//After preparing the statemant next work is to bind the param
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &issueId, 11, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding student_id", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}
	
	//Executing prepared sql statements


	ret_sql = SQLExecute(hstmt_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("deleting data from complaint", SQL_HANDLE_STMT, hstmt_sql);
		exit(-2);
	}

	printf("Successfully deleted!");
}

void del_student()
{
	SQLINTEGER stuId;
	SQLCHAR syntax[] = "DELETE FROM student WHERE stuId = ?)";


	//Getting data 
	printf("Enter the student id :");
	scanf_s("%d", &stuId);


	//preparing sql syntax
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);
	if (ret_sql != SQL_SUCCESS) {
		show_error("studnet sql syntax", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}


	ret_sql = SQLBindParameter(hstmt_sql, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &stuId, 11, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding rn paramter student", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}


	ret_sql = SQLExecute(hstmt_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("deleting data from complaint", SQL_HANDLE_STMT, hstmt_sql);
		exit(-2);
	}

	printf("Successfully deleted!");


}

void del_admin()
{

	//required data to insert
	SQLINTEGER  adminId;
	SQLCHAR* syntax = (SQLCHAR*)"DELETE FROM admin WHERE adminId = ?)";

	//Getting data
	printf("Enter admin ID: ");
	scanf_s("%d", &adminId);

	//preparing sql statement
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);

	if (ret_sql != SQL_SUCCESS) {
		show_error("preparing admin sql syntax", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//binding the parameter
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &adminId, 11, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding admin id", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//Executing sql syntax
	ret_sql = SQLExecute(hstmt_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("deleting data from student", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	printf("Successfully deleted!");
}

void del_lab()
{

	//Required data for lab
	SQLINTEGER labId;
	SQLCHAR* syntax = (SQLCHAR*)"DELETE FROM lab WHERE labId = ?";

	//getting data
	printf("Enter labId: ");
	scanf_s("%d", &labId);

	//preparing sql syntax
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);
	if (ret_sql != SQL_SUCCESS) {
		show_error("preparing sql querry lab", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//binding paramters
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 10, 0, &labId,
		10, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding labid", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//Executing stament

	ret_sql = SQLExecute(hstmt_sql);
	if (ret_sql != SQL_SUCCESS) {
		show_error("deleting datat from lab", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	printf("Successfully deleted!");

}

void del_machine()
{

	SQLINTEGER  machineId;
	SQLCHAR* syntax = (SQLCHAR*) "DELETE FROM  machine WHERE machineId = ?)";

	//getting data
	printf("Enter the computerId: ");
	scanf_s("%d", &machineId);

	//preparing sql syntax
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);
	if (ret_sql != SQL_SUCCESS) {
		show_error("preparing syntax for machine", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}


	// binding sql paramter
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &machineId,
		11, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("binding machineId for machine", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//executing syntax
	ret_sql = SQLExecute(hstmt_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("deleting data from machine", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	printf("Successfully deleted!");


}

void del_resolved_issues()
{

	//requierd data
	SQLINTEGER issueId;
	SQLCHAR* syntax = (SQLCHAR*)"DELETE FROM resolved_issue WHERE issueId = ?";


	//getting user data
	printf("Enter issueId: ");
	scanf_s("%d", &issueId);


	//preparing sql statement
	ret_sql = SQLPrepareA(hstmt_sql, syntax, SQL_NTS);

	if (ret_sql != SQL_SUCCESS) {
		show_error("preparing synatx for resolved issue", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//binding paramters
	ret_sql = SQLBindParameter(hstmt_sql, 1, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 11, 0, &issueId, 11, NULL);

	if (ret_sql != SQL_SUCCESS) {
		show_error("Binding paramter issueId for resolved_issue", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	//executing syntax
	ret_sql = SQLExecute(hstmt_sql);

	if (ret_sql != SQL_SUCCESS) {
		show_error("deleting record from resolved_issue", SQL_HANDLE_STMT, hstmt_sql);
		exit(-1);
	}

	printf("Successfully deleted!");

}




