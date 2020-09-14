#include<mysql/mysql.h>
#include<stdio.h>
#include<string.h>
#include"my_insert.h"

int insert(const char *psw,const char *usrname){
    
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
    

    if( mysql_query(sql,"USE mysqltest") ) {
        printf("ERROR IN USE\n");
        mysql_close(sql);
        return -1;
    }
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
