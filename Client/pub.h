#pragma once
#include "../Include/pub.h"
int sid;
struct sockaddr_in servaddr;
void home();
void server_connect();
void user_register(struct user *user);
void user_login(struct user *user);
void user_change();
void user_exit();
void all_init();

