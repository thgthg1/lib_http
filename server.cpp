#include<event2/event.h>
#include<event2/listener.h>
#include<event2/http.h>
#include<event2/buffer.h>
#include <event2/keyvalq_struct.h>
#include<iostream>
#include<string>
#include<signal.h>
#include <string.h>
#include<fstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "handle.h"
using namespace std;

#define WEBROOT "./html" //设置万战根目录
#define DEFAULTINDEX "html/index.html" //这个是默认请求的文件（当只输入域名时）
#define FILE_NAME "server_info"

HttpHandle handl;

void http_cb(struct evhttp_request *request, void *arg)
{
	cout << "http_cb" << endl;
	const char *uri = evhttp_request_get_uri(request);
	cout << "uri:" << uri << endl;
	string cmdtype;
	switch (evhttp_request_get_command(request)){
		case EVHTTP_REQ_GET:
			cmdtype = "GET";
			break;
		case EVHTTP_REQ_POST:
			cmdtype = "POST";
			break;
	}
	cout << "cmdtype(请求类型):" << cmdtype << endl;
	evkeyvalq *headers = evhttp_request_get_input_headers(request);
	cout << "************请求头数据************" << endl;
	for (evkeyval *p = headers->tqh_first; p != NULL; p = p->next.tqe_next){
		cout << p->key << ":" << p->value << endl;
	}
	cout<<"**********************************"<<endl;
	evbuffer *inbuf = evhttp_request_get_input_buffer(request);
	char buf[4096];
	char str[2048]={0};
	remove("base.txt");
	cout<<"========获取POST请求数据(GET请求没有数据)========"<<endl;
	while (evbuffer_get_length(inbuf)){
		int n = evbuffer_remove(inbuf, buf, sizeof(buf) - 1);
		if (n > 0){
			buf[n] = '\0';
			cout << buf << endl;
		}
		handl.POST_BUF=buf;
		handl.save_txt(buf,n);
	}
	cout<<"=================================="<<endl<<endl<<endl;
	string filepath = WEBROOT;
	filepath += uri;
	if (strcmp(uri, "/") == 0)
	{
		filepath += DEFAULTINDEX;
	}
	evkeyvalq *outhead = evhttp_request_get_output_headers(request);//outhead为响应头
	int pos = filepath.rfind('.');
	string file_type = filepath.substr(pos + 1, filepath.length() - (pos + 1));
	if (file_type == "jpg" || file_type == "gif" || file_type == "png"){
		string temp = "image/" + file_type;
		evhttp_add_header(outhead, "Content-Type", temp.c_str());
	}else if (file_type == "zip"){
		evhttp_add_header(outhead, "Content-Type", "application/zip");
	}else if (file_type == "html"){
		evhttp_add_header(outhead, "Content-Type", "text/html;charset=UTF-8");
	}else if (file_type == "css"){
		evhttp_add_header(outhead, "Content-Type", "text/css");
	}else if(file_type == "zz")
	{
		handl.Select_Order(uri);
		cout<<"respioonese"<<endl;
		evhttp_add_header(outhead, "Content-Type", "application/html");
		evbuffer *outbuf = evhttp_request_get_output_buffer(request); //返回的body
		//string json = "{\"res\":10,\"text\":\"OK\",\"data\":null}"; //模拟json数据
		cout<<handl.Response_json<<endl;
		evbuffer_add(outbuf, handl.Response_json.c_str(), handl.Response_json.length());
		evhttp_send_reply(request, HTTP_OK, "", outbuf);
		return;
	}
	FILE *fp = fopen(filepath.c_str(), "rb");
	if (!fp){
		evhttp_send_reply(request, HTTP_NOTFOUND, "", 0);
		return;

	}
	evbuffer *outbuf = evhttp_request_get_output_buffer(request);
	while (true){
		int len = fread(buf, 1, sizeof(buf), fp);
		if (len <= 0)break;
		evbuffer_add(outbuf, buf, len);
	}
	fclose(fp);
	evhttp_send_reply(request, HTTP_OK, "", outbuf);

}
int main()
{
    std::cout << "libevcent http server"<<endl; 
	event_base *base = event_base_new();
	if (base) 
	{
		cout << "event_base_new success" << endl;
	}
	evhttp *evh = evhttp_new(base);
	if (evhttp_bind_socket(evh, "0.0.0.0", 8000) != 0)
	{
		cout << "evhttp_bind_socket failed!" << endl;
	}
	else
	{

	}
	evhttp_set_gencb(evh, http_cb, 0);
	if (base)
		event_base_dispatch(base);
	if(base)
		event_base_free(base);
	if(evh)
		evhttp_free(evh);

}

