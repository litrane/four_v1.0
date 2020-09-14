#include<mysql/mysql.h>
#include<stdio.h>
#include<string.h>
#include"select.h"

int myselect(const char *password,const char* usrname){
    char client[] = "root";
    char server[] = "localhost";
    char  code[] ="root";





    MYSQL *sql = mysql_init(NULL);
    MYSQL_RES *sql_res;
    MYSQL_FIELD *field;
    

    if(sql == NULL){
        printf("ERROR IN INIT! %s\n",mysql_error(sql));
        return -1;
    }
    if(NULL   == mysql_real_connect(sql,server,client,code,NULL,0,NULL,0)  ){
        printf("ERROR IN CONNECT! %s\n",mysql_error(sql));
        mysql_close(sql);
        return -1;
    }
    /*
    if(mysql_query(sql,"CREATE DATABASE IF NOT EXSISTS mysqltest")){
        printf("ERROR IN CREATION!\n %s\n",mysql_error(sql));
        mysql_close(sql);
        return  -1;
    }
    */

    if( mysql_query(sql,"USE mysqltest") ) {
        printf("ERROR IN USE\n");
        mysql_close(sql);
        return -1;
    }
    char quest[100] = "SELECT password FROM login WHERE usrname = ";
    char tem[50];
    sprintf(tem,"'%s'",usrname);
    strcat(quest,tem);
    printf("%s\n",quest);


    if(mysql_query(sql,quest)){
        printf("ERROR IN SELECT %s",mysql_error(sql));
        mysql_close(sql);
        return -1;
    }
    sql_res = mysql_store_result(sql);

    if(mysql_num_rows(sql_res)==0){
        //printf("NO RESULT!\n");
        mysql_close(sql);
        return 0;
    }
    else {
        MYSQL_ROW row = mysql_fetch_row(sql_res);
        if(strcmp(row[0],password)==0) return 1;
	    else return 0;
    }

    
    
    //printf ("%s\n",mysql_get_server_info(sql)  );
    //printf("%s\n",mysql_get_host_info(sql));
    mysql_close(sql);
    return 1;

}
