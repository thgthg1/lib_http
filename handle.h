#include<iostream>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <map>
#include "mydata.h"
#include "json/json.h"
using namespace std;
using namespace Json;
static const string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

class HttpHandle
{
private:
    /* data */
    mydata data;
    
   
public:
    string POST_BUF;
    string Response_json;
    HttpHandle(/* args */);
    ~HttpHandle();
    void Select_Order(string uri);
    void save_txt(char* buff, size_t n);
    void write_img();
    void Json_handle(const char* str);
    void Json_test();

    static inline bool is_base64(const char c)
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
};





