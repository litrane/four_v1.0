#include<stdio.h>
#include<string.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>
#include "my_insert.h"
using namespace std;
using namespace rapidjson;
int main ()
{
    char buf[200]="{\"project\":\"rapidjson\",\"stars\":10}";
    string s=buf;
    string json =s;
    printf ("%s\n",json.c_str());
    Document dom;
    if (!dom.Parse(json.c_str()).HasParseError()) 
    {
		if (dom.HasMember("stars") && dom["stars"].IsInt()) 
        {
			printf("%d\n", dom["stars"].GetInt());
            Value&s = dom["stars"];
            s.SetInt(1);
            printf("%d\n", dom["stars"].GetInt());
            StringBuffer buffer;
            Writer<StringBuffer> Writer(buffer);
            dom.Accept(Writer);
            std::cout<<buffer.GetString()<<std::endl;
		}
    }
    else
    {
        printf("jiexi failed\n");
    }
}