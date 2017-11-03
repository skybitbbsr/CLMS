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

//Functions to be implemented
void gotoxy(short, short);
void INITHandles(SQLCHAR* ); //Test passed
void show_error(char* , SQLSMALLINT , SQLHANDLE ); //Test passed
void lodge_complaints(); //Test passed
void add_student(); //Test passed
void add_admin();// Teste passed
void free_handle(); //Test passed
void add_lab(); //Test passed
void add_machine();
void resolved_issues();

#endif