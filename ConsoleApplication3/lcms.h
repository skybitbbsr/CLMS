#pragma once

#ifndef  LCMS_H
#define LCMS_H 

#include <windows.h>
#include <stdio.h>
#include <sqlext.h>
#include <sql.h>


//Global variable required for establshing the connection to the ODBC
extern SQLHENV henv_sql;
extern SQLHDBC hdbc_sql;
extern SQLHSTMT hstmt_sql;
extern SQLRETURN ret_sql;
extern SQLWCHAR* connectionString;

//Functions for ODBC connection
void gotoxy(short, short);
void INITHandles(SQLCHAR* ); //Test passed
void free_handle(); //Test passed
void show_error(char* , SQLSMALLINT , SQLHANDLE ); //Test passed


//Authentication function or log in function
void authentiation();
char greeting(struct tm* localtime);



//object functionalites
void student_func();
void admin_func();


//Functions for adding table data
void lodge_complaints(); //Test passed
void add_student(); //Test passed
void add_admin();// Teste passed
void add_lab(); //Test passed
void add_machine();//Tesh passed
void resolved_issues(); //Test passed
void display_complaints(); //TTest Passed

void weekly_status (); 

//Functions for deleting record
void del_lodge_complaints(); 
void del_student(); //Test passed
void del_admin();//Test passed
void del_lab(); //Test passed
void del_machine();//Test passed
void del_resolved_issues(); 

#endif