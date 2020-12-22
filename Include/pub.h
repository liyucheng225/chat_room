#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <malloc.h>
#include <semaphore.h>
#include <sys/types.h>
#define MSG_CREAT 1
#define MSG_LOGIN 2
#define MSG_CHANGE 3
#define MSG_EXIT 4
#define FAILED -1
#define SUCCESS 0
#define PORT 8000

struct user
{
    int type;    //消息类型
    int passwd_len;//密码长度
    char passwd[10];  //密码
    char username[10];
    int username_len;//用户名长度
};
