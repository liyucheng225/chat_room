#include "../Include/pub.h"
#include "./pub.h"
/*
   1.注册
   输入用户名和密码
   判断是否重复
   选择秘保问题且填写答案
   注册成功
   2.登陆
   判断用户名和密码是否匹配成功
   登陆成功进入聊天模块
   3.更改密码
   输入秘保答案即可输入新密码
   4.退出*/
void all_init(){
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    inet_pton(AF_INET,"192.168.75.128",&servaddr.sin_addr.s_addr );
    servaddr.sin_port=htons(PORT);
}
void server_connect(){
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket()");
        exit(-1);

    }
    if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0){
        perror("concect()");
        exit(-1);
    }
}
void home(){
    int num=0;
    char buf[100];
    struct user *user_temp = (struct user *)malloc(100);
    struct user *user = (struct user *)malloc(100);
    while(1) {
        printf("--------------------聊天室--------------------\n");
        printf("  1.用户注册\n");
        printf("  2.用户登陆\n");
        printf("  3.忘记密码\n");
        printf("  4.退出\n");
        printf("  请输入您的选择：\n");
        scanf("%d",&num);
        switch(num) {
        case 1:
            printf("--------------------聊天室--------------------\n");
            user_register(user_temp);
            printf("name=%sname_len=%d\n",user_temp->username,user_temp->username_len);
            printf("passwa=%spass_len=%d\n",user_temp->passwd,user_temp->passwd_len);
            break;
        case 2:
            printf("--------------------聊天室--------------------\n");
            user_login(user);
            break;
        case 3:
            printf("--------------------聊天室--------------------\n");
            //user_change();
            break;
        case 4:
            printf("--------------------聊天室--------------------\n");
            //user_exit();
            break;
        default:
            printf("您的输入有误,请重新输入！\n");
            break;
        }
        fgets(buf,100,stdin);
        fflush(stdin);
    }
}
void user_register(struct user *user_temp){
    char buf[10];
    fgetc(stdin);
    printf("请输入用户名：\n");
    memset(buf,0,10);
    fgets(buf,sizeof(buf),stdin);
    strcpy(user_temp->username,buf);
    user_temp->username_len = strlen(user_temp->username);
    while(1){
        bzero(user_temp->passwd,10);
        printf("请输入密码：\n");
        memset(buf,0,10);
        fgets(buf,sizeof(buf),stdin);
        strcpy(user_temp->passwd,buf);
        user_temp->passwd_len = strlen(user_temp->passwd);
        printf("请再次输入密码：\n");
        memset(buf,0,10);
        fgets(buf,sizeof(buf),stdin);
        if (strcmp(user_temp->passwd,buf) != 0) {
            printf("输入的两次密码不一致,请重新输入!\n");
            continue;
        }
        //向服务端发送信息
        //接收服务端的消息
        printf("用户名为：%s的用户注册成功！\n",user_temp->username);
        break;
    }
}
void user_login(struct user *user){
    bzero(user->username,10);
    printf("请输入用户名：\n");
    char buf[10];
    memset(buf,0,10); 
    fgets(buf,sizeof(buf),stdin);
    strcpy(user->passwd,buf);
    user->passwd_len = strlen(user->passwd);
    bzero(user->passwd,10);
    printf("请输入密码：\n");
    memset(buf,0,10);
    fgets(buf,sizeof(buf),stdin);
    strcpy(user->passwd,buf);
    user->passwd_len = strlen(user->passwd);
    //向服务端发送信息
    //接受服务端的消息
    //判断错误类型

}
void user_change(){

}
void exit(){

}
