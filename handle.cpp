#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "handle.h"


string base64_decode(string const & encoded_string);

HttpHandle::HttpHandle(/* args */)
{
    data.InitDB("localhost","root","123","qgydb");
}

HttpHandle::~HttpHandle()
{
}


void HttpHandle::Select_Order(string uri)
{
    map<string ,int> post_order=
    {
        {"/pic.zz",1},
        {"/test.zz",2}
    };
    int val=post_order[uri];
    switch (val)
    {
    case 1:
        cout<<"save pic"<<endl;
        write_img();
        break;
    case 2:
        cout<<"test"<<endl;
        break;
    default:
        break;
    }
}

void HttpHandle::save_txt(char* buff, size_t n)
{
	FILE *fp;
	if((fp = fopen("base.txt","ab") ) == NULL )
    {
        printf("File.\n");
        exit(1);
    }
	fwrite(buff, 1, n, fp);
	fclose(fp);
}

void HttpHandle::write_img()
{
    cout<<"write img"<<endl;
	fstream f2;
    ifstream file("base.txt");
    string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	int index=content.find(",",0)+1;
	content=content.substr(index,content.length());
    string imgdecode64 = base64_decode(content);
	f2.open("savePic/out1.jpg", ios::out | ios::binary);
    f2 << imgdecode64;   
    f2.close();
    
}

string base64_decode(string const & encoded_string)
{
    int in_len = (int) encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    string ret;
    while (in_len-- && ( encoded_string[in_] != '=') && HttpHandle::is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);  
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];  
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];  
    }  
    
    return ret;  
}
