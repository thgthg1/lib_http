#include"mydata.h"

mydata::mydata()
{
    mysql = mysql_init(NULL);
	if(mysql == NULL) {
		cout << "Error: "<< mysql_error(mysql);
		exit(-1);
	}
	else
	{
		cout<<"mysql init success"<<endl;
	}
}
mydata::~mydata()
{
    if(!mysql) {
		mysql_close(mysql);
	}
}

bool mydata::InitDB(string host,string user,string pwd,string dbname)
{
    /*连接数据库*/
	if(!mysql_real_connect(mysql,host.c_str(),user.c_str(),pwd.c_str(),dbname.c_str(),0,NULL,0)) 
	{
		cout << "connect fial: "<< mysql_error(mysql);
		exit(-1);
	}
	else
	{
		cout<<"database init success!"<<endl;
	}
	return true;
}
bool mydata::ExeSQL(const char* sql)
{
	int ret = mysql_query(mysql, sql);
    if (ret) 
	{
        cout << "error!" << endl;
    } else {
        cout << "sql success!" << endl;
    }
}
void mydata::CreateTable(string table_name)
{
	string sql="create table "+table_name+" (id int(10), name char(20), passwd char(20))";
	int ret = mysql_query(mysql, sql.c_str());
	if (ret) 
	{
        cout << "CreateTable error!" << endl;
    } 
	else {
        cout << "CreateTable success!" << endl;
    }
}
bool mydata::SearchName(string name)
{
	bool b=false;
	string sql="select * from register where name='"+name+"';";
	mysql_query(mysql, sql.c_str());
	result=mysql_store_result(mysql);
	if (!result)
    {
        printf("SQL语句执行无有结果: %s\n", mysql_error(mysql));                        
    }
    if (mysql_field_count(mysql) <= 0)       
    {
        printf("查询结果为空\n");
    }
	int  num_rows=mysql_num_rows(result); 
	if(num_rows>0)
	{
		b=true;
		cout<<"search "<<name<<" ok"<<endl;
	}
	else
	{
		b=false;
		cout<<name<<"is don't have"<<endl;
	}
	
	cout<<num_rows<<endl;
	return b;
}
bool mydata::UserLog(string name, string passwd)
{
	bool b=false;
	string sql="select * from register where name='"+name+"' and passwd='"+passwd+"';";
	mysql_query(mysql, sql.c_str());
	result=mysql_store_result(mysql);
	if (!result)
    {
        printf("UserLog SQL语句执行无有结果: %s\n", mysql_error(mysql));                        
    }
    if (mysql_field_count(mysql) <= 0)       
    {
        printf("UserLog 查询结果为空\n");
    }
	int  num_rows=mysql_num_rows(result); 
	if(num_rows==1)
	{
		b=true;
	}
	else
	{
		b=false;
	}
	return b;
}
void mydata::InsertUser(string sql)
{
	ExeSQL(sql.c_str());
}