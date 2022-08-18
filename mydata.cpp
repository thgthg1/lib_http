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
        cout << "ExeSQL error!" << endl;
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
		//cout<<"search "<<name<<" ok"<<endl;
	}
	else
	{
		b=false;
		//cout<<name<<"is don't have"<<endl;
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
bool mydata::Save_Chat(string name,string time,string content)
{
	string sql="insert into chat (name,time,content) value('"+name+"','"+time+"','"+content+"');";
	//cout<<sql<<endl;
	ExeSQL(sql.c_str());
}
void mydata::Search_Chat(string name)
{
	content.clear();
	string sql="select * from chat where name='"+name+"';";
	mysql_query(mysql, sql.c_str());
	result=mysql_store_result(mysql);
	if (!result)
    {
        printf("Search_Chat SQL语句执行无有结果: %s\n", mysql_error(mysql));                        
    }
	else
	{
		int num_fields = mysql_num_fields(result);
		int  num_rows=mysql_num_rows(result); 
		cout<<num_rows<<endl;
		cout<<num_fields<<endl;
        for(int i=0;i<num_rows;i++)
		{
			vector<string> single;
            row = mysql_fetch_row(result);
            for(int j=0;j<num_fields;j++)
			{
				single.push_back(row[j]);
                //cout<<row[j]<<"\t";                 
            }
			//cout<<endl;
			content.push_back(single);
		}
	}
}
string mydata::Systime()
{
	time_t timeReal;
	time(&timeReal);
	timeReal = timeReal + 8*3600;
	tm* t = gmtime(&timeReal); 
	string syst=to_string(t->tm_year)+"-"+to_string(t->tm_mon)+"-"+to_string(t->tm_mday)
	+" "+to_string(t->tm_hour)+":"+to_string(t->tm_min)+":"+to_string(t->tm_sec)+"\n";
	return syst;
}