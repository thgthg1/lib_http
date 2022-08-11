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

#define WEBROOT "." //设置万战根目录
#define DEFAULTINDEX "html/index.html" //这个是默认请求的文件（当只输入域名时）
#define FILE_NAME "server_info"

HttpHandle handl;

void http_cb(struct evhttp_request *request, void *arg){
	cout << "http_cb" << endl;
	//1. 获取浏览器的请求信息
	const char *uri = evhttp_request_get_uri(request);
	cout << "uri:" << uri << endl;
	//请求类型 GET  POST
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
	//请求正文
	for (evkeyval *p = headers->tqh_first; p != NULL; p = p->next.tqe_next){
		cout << p->key << ":" << p->value << endl;
	}
	cout<<"**********************************"<<endl;
	//请求正文（GET为空，POST有表单信息）
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
		handl.save_txt(buf,n);
		
	}
	cout<<"=================================="<<endl<<endl<<endl;


	//2. 回复浏览器
	//状态行 消息头  响应正文 HTTP_NOTFOUND HTTP_INTERNAL
	//分析出请求的文件 uri
	//设置根目录 WEBROOT
	string filepath = WEBROOT;
	filepath += uri;
	if (strcmp(uri, "/") == 0){
		//默认加上首页文件
		filepath += DEFAULTINDEX;
	}
	//消息报头
	evkeyvalq *outhead = evhttp_request_get_output_headers(request);//outhead为响应头
	//要支持图片 CSS  js  下载zip文件
	//获取文件的后缀名
	int pos = filepath.rfind('.');
	string file_type = filepath.substr(pos + 1, filepath.length() - (pos + 1));
	if (file_type == "jpg" || file_type == "gif" || file_type == "png"){
		string temp = "image/" + file_type; //响应图片类型
		evhttp_add_header(outhead, "Content-Type", temp.c_str());
	}else if (file_type == "zip"){
		evhttp_add_header(outhead, "Content-Type", "application/zip");
	}else if (file_type == "html"){
		evhttp_add_header(outhead, "Content-Type", "text/html;charset=UTF-8");
	}else if (file_type == "css"){
		evhttp_add_header(outhead, "Content-Type", "text/css");
	}else if(file_type == "zz"){//为了知道是POST请求，我将后缀识别为.zz类型
		//这里表示是POST请求
		handl.Select_Order(uri);
		evhttp_add_header(outhead, "Content-Type", "application/html");
		evbuffer *outbuf = evhttp_request_get_output_buffer(request); //返回的body
		string json = "{\"res\":100,\"text\":\"OK\",\"data\":null}"; //模拟json数据
		evbuffer_add(outbuf, json.c_str(), json.length());
		evhttp_send_reply(request, HTTP_OK, "", outbuf);
		return;
	}
	//读取html文件返回正文
	FILE *fp = fopen(filepath.c_str(), "rb");
	if (!fp){
		evhttp_send_reply(request, HTTP_NOTFOUND, "", 0);//返回未找到
		//这里可以自定义未找到页面
		return;

	}
	evbuffer *outbuf = evhttp_request_get_output_buffer(request); //返回的body
	while (true){
		int len = fread(buf, 1, sizeof(buf), fp); //不断读取，解决文件过大一次性读不完的问题
		if (len <= 0)break;
		evbuffer_add(outbuf, buf, len);
	}
	fclose(fp);
	evhttp_send_reply(request, HTTP_OK, "", outbuf);

}
int main(){
#ifdef _WIN32
	//初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);
#else
	//忽略管道信号， 发送数据给已关闭的socket
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)return 1;
#endif
    std::cout << "http服务器\n"; 
	event_base *base = event_base_new();
	if (base) {
		cout << "event_base_new success" << endl;
	}
	evhttp *evh = evhttp_new(base);
	if (evhttp_bind_socket(evh, "0.0.0.0", 8000) != 0){//全部IP可访问
		cout << "evhttp_bind_socket failed!" << endl;
	}
	//3. 设定回调函数
	evhttp_set_gencb(evh, http_cb, 0);
	//事件分发处理
	if (base)
		event_base_dispatch(base);
	if(base)
		event_base_free(base);
	if(evh)
		evhttp_free(evh);
#ifdef _WIN32
	WSACleanup();
#endif

}

