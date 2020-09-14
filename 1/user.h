#include<mysql/mysql.h>
#include<stdio.h>
#include<string.h>


MYSQL* creat(char* dbname){
    char client[] = "root";
    char server[] = "localhost";
    char  code[] ="root";
    MYSQL* sql = mysql_init(NULL);
    MYSQL_RES * sql_res = NULL;
    MYSQL_ROW row;
    if(sql == NULL) {
        printf("%s\n",mysql_error(sql));
        mysql_close(sql);
        return NULL;
    }
    if(mysql_real_connect(sql,server,client,code,dbname,0,NULL,0) == NULL ){
        printf("%s\n",mysql_error(sql));
        mysql_close(sql);
        return NULL;
    }
    return sql;
}


int usr_insert(const char *psw,const char *usrname){
    MYSQL *sql = creat("mysqltest");

    char quest[] = "INSERT INTO login (usrname,  password) VALUES ";
    char values[200];
    sprintf(values,"('%s','%s')",usrname,psw);
    strcat(quest,values);

    
    if(mysql_query(sql,quest)){
        printf("ERROR IN INSERT! %s\n",mysql_error(sql));
        mysql_close(sql);
        return -1;  
    }
    
    printf ("%s\n",mysql_get_server_info(sql)  );
    printf("%s\n",mysql_get_host_info(sql));
    mysql_close(sql);
    return 0;

}

int usr_select(const char *password,const char* usrname){
    MYSQL *sql = creat("mysqltest");
    MYSQL_RES *sql_res;
    MYSQL_FIELD *field;
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
        mysql_close(sql);
        return 0;
    }
    else {
        MYSQL_ROW row = mysql_fetch_row(sql_res);
        if(strcmp(row[0],password)==0) return 1;
	    else return 0;
    }
    mysql_close(sql);
    return 1;
}
int usr_check(const char* usrname){
    MYSQL *sql = creat("mysqltest");
    MYSQL_RES *sql_res;
    char quest[100] = "SELECT * FROM login WHERE usrname = ";
    char tem[50];
    sprintf(tem,"'%s'",usrname);
    strcat(quest,tem);
    if(mysql_query(sql,quest)){
        printf("ERROR IN SELECT %s",mysql_error(sql));
        mysql_close(sql);
        return -1;
    }
    sql_res = mysql_store_result(sql);

    if(mysql_num_rows(sql_res)==0){
        mysql_close(sql);
        return 0;
    }
    mysql_close(sql);
    return 1;
}
