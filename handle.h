#include<iostream>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <map>
using namespace std;
static const string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

class HttpHandle
{
private:
    /* data */
     
   
public:
//string base64_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    HttpHandle(/* args */);
    ~HttpHandle();
    void Select_Order(string uri);
    void save_txt(char* buff, size_t n);
    void write_img();
    static inline bool is_base64(const char c)
    {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }
};





