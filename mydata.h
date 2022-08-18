#ifndef MYDATA_H
#define MYDATA_H
#include<mysql/mysql.h>
#include<iostream>
#include <time.h>
#include<vector>
using namespace std;

class mydata
{
private:
    /* data */
    MYSQL* mysql;
	MYSQL_ROW row;
	MYSQL_RES* result;
	MYSQL_FIELD* field;
public:
    vector<vector<string>> content;
    mydata(/* args */);
    ~mydata();
    bool InitDB(string host,string user,string pwd,string dbname);
    void CreateTable(string table_name);
    void InsertUser(string sql);
    void Search_Chat(string name);
    bool SearchName(string name);
    bool UserLog(string name, string passwd);
    bool Save_Chat(string name,string time,string content);
    bool ExeSQL(const char* sql);
    string Systime();
};


#endif
