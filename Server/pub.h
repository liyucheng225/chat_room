#pragma once
#include "../Include/pub.h"
#include <mysql/mysql.h>

void allinit();
void server_connect();
void database_handle(struct user *user,int cid);
void database_connect(MYSQL *mysql);
int database_search_user(MYSQL *mysql,struct user *user);
int database_search_name(MYSQL *mysql, char *name);
int database_insert_data_to_clientinfo(MYSQL *MYSQL,struct user *user);
void database_close_connection(MYSQL *mysql);
int database_search_question(MYSQL *mysql,char *name,struct quest *que );
int sid;
struct sockaddr_in seraddr;
struct sockaddr_in cliaddr;
socklen_t serlen;
socklen_t clilen;
