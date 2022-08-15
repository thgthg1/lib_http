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
	if(!mysql_real_connect(mysql,host.c_str(),user.c_str(),pwd.c_str(),dbname.c_str(),0,NULL,0)) {
		cout << "connect fial: "<< mysql_error(mysql);
		exit(-1);
	}
	return true;
}