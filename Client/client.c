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
    if ((sid = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket()");
        exit(-1);

    }
    if (connect(sid,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0){
        perror("concect()");
        exit(-1);
    }
    printf("success server\n");
}
void home(){
    int num=0;
    char buf[100];
    struct user *user_temp = (struct user *)malloc(1000);
    struct user *user = (struct user *)malloc(1000);
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
            printf("请按任意键继续:");
            fgetc(stdin);
            break;
        case 2:
            printf("--------------------聊天室--------------------\n");
            //user_login(user);
            break;
        case 3:
            printf("--------------------聊天室--------------------\n");
            user_change();
            printf("请按任意键继续:");
            fgetc(stdin);
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
    char buf1[10];
    int rebuf;
    int relen;
    char quest;
    char answer[10];
    char quest1[]={"1.小学同桌是谁？"};
    char quest2[]={"2.数学最高分？"};
    char quest3[]={"3.出生街道是？"};
    fgetc(stdin);
    while(1){
        printf("请输入用户名：\n");
        bzero(user_temp,1000);
        memset(buf,0,10);
        fgets(buf,sizeof(buf),stdin);
        memcpy(user_temp->username,buf,strlen(buf)-1);
        user_temp->type = NAME_CREAT;
        if (send(sid,user_temp,1000,0) < 0) {
            perror("send()");
            exit(-1);
        }
        printf("user_temp=%s",user_temp->username);
        if ((relen = recv(sid,&rebuf,sizeof(rebuf),0)) < 0) {
            perror("recv");
            exit(-1);
        }
        if (relen > 0) {
            if(rebuf == SQL_FAILED) {
                break;
            }else if (rebuf == SQL_SUCCESS) {
                printf("用户名已经注册，请重新输入！\n");
            }
        }
        fgetc(stdin);
    }
    while(1){
        quest = 0;
        bzero(user_temp->passwd,10);
        printf("请输入密码：\n");
        memset(buf,0,10);
        fgets(buf,sizeof(buf),stdin);
        memcpy(user_temp->passwd,buf,strlen(buf)-1);
        printf("ps=%s",user_temp->passwd);
        printf("请再次输入密码：\n");
        memset(buf,0,10);
        memset(buf1,0,10);
        fgets(buf,10,stdin);
        memcpy(buf1,buf,strlen(buf)-1);
        printf("buf=%s",buf1);
        if (strcmp(user_temp->passwd,buf1) != 0) {
            printf("输入的两次密码不一致,请重新输入!\n");
            continue;
        }
        printf("请选择你的密保问题：\n");
        printf("%s\n%s\n%s\n",quest1,quest2,quest3);
        printf("请输入你的选择：\n");
        quest = fgetc(stdin);
        if(atoi(&quest) == 1) {
            memcpy(user_temp->question,quest1,strlen(quest1));
            printf("%s\n",user_temp->question);
        }else if (atoi(&quest) ==2) {
            memcpy(user_temp->question,quest2,strlen(quest2));
            printf("%s\n",user_temp->question);
        }else if(atoi(&quest) ==3) {
            memcpy(user_temp->question,quest3,strlen(quest2));
            printf("%s\n",user_temp->question);
        }else {
            printf("输入错误，请重新输入！\n");
            continue;
        }
        printf("%s\n",user_temp->question);
        fgetc(stdin);
        printf("请输入您的答案：\n");
        memset(answer,0,10);
        fgets(answer,10,stdin);
        printf("%s\n",user_temp->question);
        memcpy(user_temp->answer,answer,strlen(answer)-1);
        printf("%s\n",user_temp->question);
        break;
    }
    //向服务端发送信息
    user_temp->type = USER_CREAT;
    if((relen=send(sid,user_temp,1000,0)) < 0) {
        perror("recv()");
        exit(-1);
    }
    printf("answer=%s\n",user_temp->answer);
    printf("name=%s\n",user_temp->username);
    printf("quest=%s\n",user_temp->question);
    printf("ans=%s\n",user_temp->answer);
    //接收服务端的消息
    if ((relen = recv(sid,&rebuf,sizeof(buf),0)) < 0) {
        perror("recv");
        exit(-1);
    }
}

void user_login(struct user *user){
    int relen;
    struct quest *que=(struct quest *)malloc(100);
    char buf[10];
    bzero(user->username,10);
    printf("请输入用户名：\n");
    memset(buf,0,10); 
    fgets(buf,sizeof(buf),stdin);
    memcpy(user->passwd,buf,strlen(buf)-1);
    bzero(user->passwd,10);
    printf("请输入密码：\n");
    memset(buf,0,10);
    fgets(buf,sizeof(buf),stdin);
    memcpy(user->passwd,buf,strlen(buf)-1);
    if((relen=send(sid,user,1000,0)) < 0) {
        perror("recv()");
        exit(-1);
    }
    if ((relen = recv(sid,que,100,0)) < 0) {
        perror("recv");
        exit(-1);
    }
    //向服务端发送信息
    //接受服务端的消息
    //判断错误类型

}
void user_change(struct user *user){
    int relen;
    int rebuf;
    struct quest *que=(struct quest *)malloc(100);
    char buf[10];
    char buf1[10];
    while(1){
        bzero(user->username,10);
        bzero(que->question,20);
        bzero(que->answer,10);
        printf("请输入用户名：\n");
        memset(buf,0,10); 
        fgets(buf,sizeof(buf),stdin);
        memcpy(user->passwd,buf,strlen(buf)-1);
        if((relen=send(sid,user,1000,0)) < 0) {
            perror("recv()");
            exit(-1);
        }
        if ((relen = recv(sid,&rebuf,sizeof(rebuf),0)) < 0) {
            perror("recv");
            exit(-1);
        }
            if ((relen = recv(sid,que,100,0)) < 0) {
                perror("recv");
                exit(-1);
            }
        if(rebuf==SQL_FAILED){
            printf("账号错误,请重新输入!\n");
        }else if(rebuf == SQL_SUCCESS) {
            printf("您的密保问题是：%s\n",que->question);
            printf("请回答您的密保问题：\n");
            memcpy(buf1,buf,strlen(buf)-1);
            memset(buf,0,10); 
            fgets(buf,10,stdin);
            if(strcmp(que->answer,buf1)==0) {
                printf("回答正确！\n");
                bzero(user->passwd,10);
                bzero(user->question,20);
                bzero(user->answer,10);
                printf("请输入新密码：\n");
                memset(buf,0,10); 
                fgets(buf,sizeof(buf),stdin);
                memcpy(user->passwd,buf,strlen(buf)-1);
                strcpy(user->question,que->question);
                strcpy(user->answer,que->answer);
                user->type = NAME_CREAT;
                if((relen=send(sid,user,1000,0)) < 0) {
                    perror("recv()");
                    exit(-1);
                }
                break;
            }
        }
    }

}
//void exit(){

//}
