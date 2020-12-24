#pragma once
#include "../Include/pub.h"
int sid;
struct sockaddr_in servaddr;
void home();
void server_connect();
void user_register(struct user *user);
void user_login(struct user *user);
void user_change(struct user *user);
void user_exit();
void all_init();
void chat_room(struct user *user);
void group_chat(struct user *user);
void file_transfer(struct user *user);
void online_user(struct user *user);
void ban_user(struct user *user);
void pick_user(struct user *user);
void kick_user(struct user *user);
void private_chat(struct user *user);
int id_online(struct user *user);
