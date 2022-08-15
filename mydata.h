#ifndef MYDATA_H
#define MYDATA_H
#include<mysql/mysql.h>
#include<iostream>
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
    mydata(/* args */);
    ~mydata();
    bool InitDB(string host,string user,string pwd,string dbname);
    bool ExeSQL(string sql);
};


#endif
