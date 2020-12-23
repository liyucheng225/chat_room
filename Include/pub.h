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
#include <sys/epoll.h>
#define USER_CREAT 1
#define USER_LOGIN 2
#define USER_CHANGE 3
#define USER_EXIT 4
#define NAME_FAILED -1
#define PWSD_FAILED -2
#define SUCCESS 0
#define PORT 8000
#define SQL_SUCCESS 33
#define SQL_FAILED -3
#define NAME_CREAT 5
struct quest {
    char question[20];
    char answer[10];
};

struct user
{
    int type;    //消息类型
    char passwd[10];  //密码
    char username[10];
    char question[20];
    char answer[10];
};
