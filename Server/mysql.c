#include "../Include/pub.h"
#include "./pub.h"
void database_connect(MYSQL *mysql) {
    mysql_init(mysql);
    if (!mysql_real_connect(mysql,"localhost","root","990306","chat_room",0,NULL,0)) {
        printf("Error connecting tp database:%s\n",mysql_error(mysql));
    }else{
        printf("数据库连接成功！\n");
    }
}
int  database_insert_data_to_clientinfo(MYSQL *mysql,struct user *user ) {
    int res;
    char buf[1024];
    sprintf(buf,"insert into user(name,password,question,answer) values ('%s','%s','%s','%s')",user->username,user->passwd,user->question,user->answer);
    printf("%s",buf);
    res = mysql_query(mysql,buf);
    if(!res) {
        printf("insert %lu rows\n",(unsigned long)mysql_affected_rows(mysql));
        return SQL_SUCCESS;
    }else {
        fprintf(stderr,"insert error %d:%s\n",mysql_errno(mysql),mysql_error(mysql));
        return SQL_FAILED;
    }
}

/*int databse_search_server_info(MYSQL mysql,Sinfo *server,int *returnCount) {

  }*/
int database_search_user(MYSQL *mysql, struct user *user){
    int flag_pwd=0;
    int flag_usm=0;
    char buf[1024];      /*定义一个存放数据库操作指令的字符串*/
    sprintf(buf,"select * from user where name='%s'",user->username);
    MYSQL_RES *res;                                             /*res用来保存查询到的结果集*/
    MYSQL_ROW row;                                              /*用来储存每一行的数据*/
    if (mysql_query(mysql, buf)){                /*调用数据库的查询语句函数，成功返回0，失败返回其他*/
        printf("Failed to query user!\n");
        return SQL_FAILED;
    }else{
        printf("Query made ........\n");
        res = mysql_use_result(mysql);                           /*初始化逐行的结果集检索*/
        if (res){
            row = mysql_fetch_row(res);                     /*从结果集中获取下一行*/
            for (int t = 0; t <(int)mysql_num_fields(res);t++){ /*返回结果集中的列数*/
                if (t == 0){    
                    if(strcmp(user->username,row[t]) == 0) {
                        flag_usm = 1;                    
                    }else {
                        return NAME_FAILED;
                    }
                }else if (t == 1){
                    printf("psss=%s",row[t]);
                    if(strcmp(user->passwd,row[t]) == 0) {
                        flag_pwd = 1;
                    }else {
                        return PWSD_FAILED;
                    }
                }
                if(flag_pwd==1&&flag_usm==1){
                    return SQL_SUCCESS;
                }
            }
        }  
    }
    mysql_free_result(res);         
    return SQL_SUCCESS;
}
int database_search_name(MYSQL *mysql,char *name ){
    char query_server_info[] = "select * from user;";     /*定义一个存放数据库操作指令的字符串*/
    MYSQL_RES *res;                                             /*res用来保存查询到的结果集*/
    MYSQL_ROW row;                                              /*用来储存每一行的数据*/
    int count = 0;

    if (mysql_query(mysql, query_server_info)){                /*调用数据库的查询语句函数，成功返回0，失败返回其他*/
        printf("Failed to query user!\n");
        mysql_free_result(res);
    }else{
        printf("Query made ........\n");
        res = mysql_use_result(mysql);                           /*初始化逐行的结果集检索*/
        if (res){
            for (int r = 0; r < (int )mysql_field_count(mysql); r++){/*返回上次执行语句的的结果列的数目*/
                row = mysql_fetch_row(res);                     /*从结果集中获取下一行*/
                if (row == NULL){                               /*若下一行为空，则跳出循环*/
                    break;
                }
                count++;                                        /*记录查询到的客户数量*/
                for (int t = 0; t < (int )mysql_num_fields(res);t++){ /*返回结果集中的列数*/
                    if (t == 0){    
                        if(strcmp(name,row[t]) == 0) {
                            return SQL_SUCCESS;
                        }
                    }
                }  
            }
            mysql_free_result(res);         
        }
    }
    return SQL_FAILED;
}
int database_search_question(MYSQL *mysql,char *name,struct quest *que ){
    char query_server_info[] = "select * from user;";     /*定义一个存放数据库操作指令的字符串*/
    MYSQL_RES *res;                                             /*res用来保存查询到的结果集*/
    MYSQL_ROW row;                                              /*用来储存每一行的数据*/
    int count = 0;

    if (mysql_query(mysql, query_server_info)){                /*调用数据库的查询语句函数，成功返回0，失败返回其他*/
        printf("Failed to query user!\n");
        mysql_free_result(res);
    }else{
        printf("Query made ........\n");
        res = mysql_use_result(mysql);                           /*初始化逐行的结果集检索*/
        if (res){
            for (int r = 0; r < (int )mysql_field_count(mysql); r++){/*返回上次执行语句的的结果列的数目*/
                row = mysql_fetch_row(res);                     /*从结果集中获取下一行*/
                if (row == NULL){                               /*若下一行为空，则跳出循环*/
                    break;
                }
                count++;                                        /*记录查询到的客户数量*/
                for (int t = 0; t < (int )mysql_num_fields(res);t++){ /*返回结果集中的列数*/
                    if (t == 0){    
                        if(strcmp(name,row[t]) == 0) {
                            strcpy(que->question,row[t+2]);
                            strcpy(que->answer,row[t+3]);
                            return SQL_SUCCESS;
                        }
                    }
                }  
            }
            mysql_free_result(res);         
        }
    }
    return SQL_FAILED;
}
void database_close_connection(MYSQL *mysql) {
    mysql_close(mysql);
}
