#include<stdio.h>
#include<string>
#include<mysql/mysql.h>
#include"user.h"
#include"rapidjson/stringbuffer.h"
#include"rapidjson/writer.h"
#include"rapidjson/rapidjson.h"
#include<iostream>

 

int email_insert(const char * time,const char* send,const char* receive,const char* theme,const char save_statue,const char  send_statue){
    MYSQL* email_sql;
    email_sql = creat("mysqltest");
    if(usr_check(receive)==0){
        printf("ERROR RECEIVER!\n");
        return -1;
    }
    char quest[500] = "INSERT INTO email (send,time,receive,save_statue,send_statue,theme) VALUES ";
    char values[500];
    sprintf(values,"('%s','%s','%s','%c','%c','%s')",send,time,receive,save_statue,send_statue,theme);
    strcat(quest,values);
    if(mysql_query(email_sql,quest)){
        printf("%s\n",mysql_error(email_sql));
        return 0;
    }
    printf("email insertion successful\n");
    return 1;
}
MYSQL_RES* email_select_receive(const char* receive){
    MYSQL_RES * sql_res;
    MYSQL* sql;
    sql = creat("mysqltest");
    char quest[500] ;
    sprintf(quest,"SELECT * FROM email WHERE receive = '%s' ",receive);
    if(mysql_query(sql,quest)){
        printf("%s\n",mysql_error(sql));
    }
    sql_res = mysql_store_result(sql);
}


std::string Sql_Res_To_Json(MYSQL_RES * sql_res){
    int flag  = mysql_num_rows(sql_res);
    if(flag == 0){
        printf("NO EMAILS \n");
        return 0;
    }
    MYSQL_ROW row;

    rapidjson::StringBuffer s;
    rapidjson:: Writer<rapidjson::StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("nums");
    writer.Int(flag);
    writer.Key("emails");
    writer.StartArray();
    int  i = 0;
    while (row = mysql_fetch_row(sql_res)){
        writer.StartObject();
        writer.Key("num");writer.Int(i);
        i++;
        writer.Key("send"); writer.String(row[0]);
        writer.Key("time"); writer.String(row[1]);
        writer.Key("theme");writer.String(row[5]);
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
    return s.GetString();
}