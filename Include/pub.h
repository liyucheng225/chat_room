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
#define NAME_CREAT 5
#define CHAT 6
#define SUCCESS 7
#define SQL_SUCCESS 8
#define ROOM_MASTER 9
#define BAN_USER 10
#define PICK_USER 11
#define FILE_SEND 12
#define ONLINE_USER 13
#define PRIVATE_CHAT 14
#define KICK_USER 15
#define ROOM_USER 16
#define USER_ONLINE 17
#define CHAT_ROOM 18
#define IS_MASTER 19
#define IS_ONLINE 20
#define NAME_FAILED -1
#define PWSD_FAILED -2
#define SQL_FAILED -3
#define PORT 8000
struct quest {
    char question[20];
    char answer[10];
};

struct cid{
    int online;
    int  type;
    int clid;
    int num;
    int is_ban;
    char name[10];
}cid[50];

struct user
{
    int type;    //消息类型
    char passwd[10];  //密码
    char username[10];
    char question[20];
    char answer[10];
    char data[1024];
};
