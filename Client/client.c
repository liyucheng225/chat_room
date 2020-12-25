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
char message_p[5][1024];
char message_g[5][1024];
int private_num=0;
int group_num=0;
struct mesg mesg;
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
    char num;
    char buf[100];
    struct user *user_temp = (struct user *)malloc(2048);
    struct user *user = (struct user *)malloc(2048);
    while(1) {
        printf("***********************************************\n");
        printf("\033[5m");
        printf("\033[1;33m");
        printf("--------------------聊天室--------------------\n");
        printf("  1.用户注册\n");
        printf("  2.用户登陆\n");
        printf("  3.忘记密码\n");
        printf("  4.退出\n");
        printf("\033[0m");
        printf("***********************************************\n");
        printf("\033[0;36m");
        printf("  请输入您的选择：\n");
        num=fgetc(stdin);
        getchar();
        switch(atoi(&num)) {
        case 1:{
                   printf("--------------------聊天室--------------------\n");
                   user_register(user_temp);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 2:{
                   printf("--------------------聊天室--------------------\n");
                   user_login(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 3:{
                   printf("--------------------聊天室--------------------\n");
                   user_change(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 4:{
                   printf("--------------------聊天室--------------------\n");
                   user_exit();
               }
               return;
        default:
               printf("您的输入有误,请重新输入！\n");
               break;
        }
        printf("\033[0;36m");
        printf("\033[0m");
        fflush(stdin);
    }
}
void user_register(struct user *user_temp){
        printf("\033[0;36m");
    char buf[10];
    char buf1[10];
    int rebuf;
    int relen;
    char quest;
    char answe[10];
    char quest1[]={"1.小学同桌是谁？"};
    char quest2[]={"2.数学最高分？"};
    char quest3[]={"3.出生街道是？"};
    while(1){
        printf("请输入用户名：\n");
        bzero(user_temp,2048);
        memset(buf,0,10);
        fgets(buf,sizeof(buf),stdin);
        memcpy(user_temp->username,buf,strlen(buf)-1);
        user_temp->type = NAME_CREAT;
        if (send(sid,user_temp,2048,0) < 0) {
            perror("send()");
            exit(-1);
        }
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
    fflush(stdin);
    while(1){
        quest = 0;
        bzero(user_temp->passwd,10);
        printf("请输入密码：\n");
        memset(buf,0,10);
        fgets(buf,sizeof(buf),stdin);
        memcpy(user_temp->passwd,buf,strlen(buf)-1);
        printf("请再次输入密码：\n");
        memset(buf,0,10);
        memset(buf1,0,10);
        fgets(buf,10,stdin);
        memcpy(buf1,buf,strlen(buf)-1);
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
        getchar();
        printf("请输入您的答案：\n");
        memset(answe,0,10);
        fgets(answe,10,stdin);
        memcpy(user_temp->answer,answe,strlen(answe)-1);
        break;
    }
    //向服务端发送信息
    user_temp->type = USER_CREAT;
    if((relen=send(sid,user_temp,2048,0)) < 0) {
        perror("recv()");
        exit(-1);
    }
        printf("\033[0m");
}

void user_login(struct user *user){
        printf("\033[0;36m");
    int relen;
    int rebuf;
    char buf[10];
    fflush(stdin);
    bzero(user->username,10);
    printf("请输入用户名：\n");
    memset(buf,0,10); 
    fgets(buf,10,stdin);
    memcpy(user->username,buf,strlen(buf)-1);
    bzero(user->passwd,10);
    printf("请输入密码：\n");
    memset(buf,0,10);
    fflush(stdin);
    fgets(buf,10,stdin);
    memcpy(user->passwd,buf,strlen(buf)-1);
    user->type = USER_LOGIN;
    if((relen=send(sid,user,2048,0)) < 0) {
        perror("recv()");
        exit(-1);
    }
    if ((relen = recv(sid,&rebuf,sizeof(rebuf),0)) < 0) {
        perror("recv");
        exit(-1);
    }
    if(rebuf == NAME_FAILED) {
        printf("用户名错误,请重新输入\n");
    }else if(rebuf == PWSD_FAILED) {
        printf("密码错误请,重新输入\n");
    }else if(rebuf == SQL_SUCCESS) {
        printf("验证成功，进入聊天界面\n");
        chat_room(user);
    }else if(rebuf == USER_ONLINE){
        printf("请不要重新登陆\n");
    }else if(rebuf == SQL_FAILED) {
        printf("信息错误请重新输入");
    }
        printf("\033[0m");
}
void user_change(struct user *user){
        printf("\033[0;36m");
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
        memcpy(user->username,buf,strlen(buf)-1);
        user->type = USER_CHANGE;
        if((relen=send(sid,user,2048,0)) < 0) {
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
            continue;
        }else if(rebuf == SQL_SUCCESS) {
            printf("您的密保问题是：\n%s\n",que->question);
            printf("请回答您的密保问题：\n");
            memcpy(buf1,buf,strlen(buf)-1);
            memset(buf,0,10); 
            memset(buf1,0,10); 
            fflush(stdin);
            fgets(buf,10,stdin);
            memcpy(buf1,buf,strlen(buf)-1);
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
                user->type = USER_CREAT;
                if((relen=send(sid,user,2048,0)) < 0) {
                    perror("recv()");
                    exit(-1);
                }
                break;
            }else {
                printf("回答错误!");
            }
        }
    }
        printf("\033[0m");

}
void user_exit(struct user *user){
        printf("\033[0;36m");
    int relen;
    user->type = USER_EXIT;
    if((relen=send(sid,user,2048,0)) < 0) {
        perror("recv()");
        exit(-1);
    }
    close(sid);
        printf("\033[0m");
}
void *recv_select(void *arg){
    int relen;
    while(1){
        if((relen=recv(sid,&mesg,2048,0)) < 0) {
            perror("recv()");
            exit(-1);
        }
        if(relen > 0){
            if(mesg.type == CHAT){
                strcpy(message_g[group_num],mesg.group_mesg);
                group_num++;
                if(private_num==5){
                    for(int j=0;j<group_num;j++){
                        memset(message_g[j],0,1024);
                    }
                    group_num=0;
                }
                else if(mesg.type == PRIVATE_CHAT){
                    strcpy(message_p[private_num],mesg.private_mesg);
                    private_num++;
                    group_num++;
                    if(private_num==5){
                        for(int j=0;j<private_num;j++){
                            memset(message_p[j],0,1024);
                        }
                        private_num=0;
                    }
                }
            }
        }
    }
}
void chat_room(struct user *user){
    int type;
    user->type = CHAT_ROOM;
    send(sid,user,2048,0);
    recv(sid,&type,sizeof(type),0);
    if(type == USER_ONLINE) {
        printf("该用户已在线!\n");
        return;
    }
    char num;
    pthread_t thread;
    if(pthread_create(&thread,0,recv_select,0) < 0) {
        perror("pthread_create()");
        exit(-1);
    }
    while(1) {
        num=0;
        printf("\033[5m");
        printf("\033[1;33m");
        printf("--------------------聊天室--------------------\n");
        printf("  1.查看在线人数\n");
        printf("  2.群聊\n");
        printf("  3.文件传输\n");
        printf("  4.禁言\n");
        printf("  5.解禁\n");
        printf("  6.踢人\n");
        printf("  7.私聊\n");
        printf("  8.退出\n");
        printf("  请输入您的选择：\n");
        num=fgetc(stdin);
        fgetc(stdin);
        switch(atoi(&num)) {
        case 1:{
                   printf("--------------------聊天室--------------------\n");
                   printf("----------------查看在线人数--------------------\n");
                   online_user(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 2:{
                   printf("--------------------聊天室--------------------\n");
                   printf("--------------------群聊--------------------\n");
                   group_chat(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 3:{
                   printf("--------------------聊天室--------------------\n");
                   printf("------------------文件传输--------------------\n");
                   //        file_transfer(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 4:{
                   printf("--------------------聊天室--------------------\n");
                   printf("--------------------禁言--------------------\n");
                   ban_user(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 5:{
                   printf("--------------------聊天室--------------------\n");
                   printf("--------------------解禁--------------------\n");
                   pick_user(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 6:{
                   printf("--------------------聊天室--------------------\n");
                   printf("--------------------踢人--------------------\n");
                   kick_user(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 7:{
                   printf("--------------------聊天室--------------------\n");
                   printf("--------------------私聊--------------------\n");
                                   private_chat(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 8:{
                   printf("--------------------聊天室--------------------\n");
                   printf("--------------------退出--------------------\n");
                   exit_user(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        default:
               printf("您的输入有误,请重新输入！\n");
               break;
        }
        printf("\033[0m");
        fflush(stdin);
    }
}
void group_chat(struct user *user){
        printf("\033[0;36m");
    char num;
    char buf[1024];
    user->type = IS_BAN;
    int type1;
    if(send(sid,user,2048,0) < 0) {
        perror("recv()");
        exit(-1);
    }
    printf("ban1\n");
    if(recv(sid,&type1,sizeof(type1),0) < 0) {
        perror("recv()");
        exit(-1);
    }
    printf("ban2\n");
    if(type1 == IS_BAN){
        printf("您已被群主禁言!\n");
        return;
    }
    printf("ban3\n");
    while(1){
        printf("1.发送/2.退出/3.刷新：\n");
        num=fgetc(stdin);
        getchar();
        if(num=='1'){
            printf("请输入发送的内容,输入Exit退出：\n");
            fgets(buf,1024,stdin);
            if(strcmp(buf,"Exit\n") == 0) {
                break;
            }
            strcpy(user->data,buf);
            user->type = CHAT;
            if(send(sid,user,2048,0) < 0) {
                perror("recv()");
                exit(-1);
            }
        }else if(num=='2'){
            break;
        }else if(num=='3'){
            for(int j=0;j<group_num;j++){
                printf("%s",message_g[j]);
            }
        }
        fflush(stdin);
    }
        printf("\033[0m");
}
void online_user(struct user *user){
        printf("\033[0;36m");
    int online_num=0;
    /*if(id_online(user)<0){
        printf("您以被群主强制下线!\n");
        return;
    }*/
    user->type = ONLINE_USER;
    if(send(sid,user,2048,0) < 0) {
        perror("recv()");
        exit(-1);
    }
    if(recv(sid,&online_num,sizeof(online_num),0) < 0) {
        perror("send()");
        exit(-1);
    }
    printf("当前在线人数为：%d\n",online_num);
        printf("\033[0m");

}
void ban_user(struct user *user){
        printf("\033[0;36m");
    int type;
    char buf[10];
    user->type = IS_MASTER;
    if(send(sid,user,2048,0) < 0) {
        perror("send()");
        exit(-1);
    }
    if(recv(sid,&type,sizeof(type),0) < 0) {
        perror("recv()");
        exit(-1);
    }
    if(type != ROOM_MASTER){
        printf("你不是群主，想什么呢!\n");
        return;
    }else if(type == ROOM_MASTER) {
        printf("请输入要禁言的用户名！\n");
        fgets(buf,10,stdin);
        memset(user->username,0,10);
        memcpy(user->username,buf,strlen(buf)-1);
        user->type = BAN_USER;
        if(send(sid,user,2048,0) < 0) {
            perror("send()");
            exit(-1);
        }
        if(recv(sid,&type,sizeof(type),0) < 0) {
            perror("recv()");
            exit(-1);
        }
        if(type == NAME_FAILED) {
            printf("用户名错误!\n");
        }else if(type == SUCCESS) {
            printf("禁言成功!\n");
        }
    }
        printf("\033[0m");

}
void pick_user(struct user *user){
        printf("\033[0;36m");
    int type;
    char buf[10];
    user->type = IS_MASTER;
    if(send(sid,user,2048,0) < 0) {
        perror("send()");
        exit(-1);
    }
    if(recv(sid,&type,sizeof(type),0) < 0) {
        perror("recv()");
        exit(-1);
    }
    if(type != ROOM_MASTER){
        printf("你不是群主，想什么呢!\n");
        return;
    }else if(type == ROOM_MASTER) {
        printf("请输入要解禁的用户名！\n");
        fgets(buf,10,stdin);
        memset(user->username,0,10);
        memcpy(user->username,buf,strlen(buf)-1);
        user->type = PICK_USER;
        if(send(sid,user,2048,0) < 0) {
            perror("send()");
            exit(-1);
        }
        if(recv(sid,&type,sizeof(type),0) < 0) {
            perror("recv()");
            exit(-1);
        }
        if(type == NAME_FAILED) {
            printf("用户名错误!\n");
        }else if(type == SUCCESS) {
            printf("解禁成功!\n");
        }
    }
        printf("\033[0m");


}


void kick_user(struct user *user){
        printf("\033[0;36m");
    int type;
    char buf[10];
    user->type = IS_MASTER;
    if(send(sid,user,2048,0) < 0) {
        perror("send()");
        exit(-1);
    }
    if(recv(sid,&type,sizeof(type),0) < 0) {
        perror("recv()");
        exit(-1);
    }
    if(type != ROOM_MASTER){
        printf("你不是群主，想什么呢!\n");
        return;
    }else if(type == ROOM_MASTER) {
        printf("请输入要踢出的用户名！\n");
        fgets(buf,10,stdin);
        memset(user->username,0,10);
        memcpy(user->username,buf,strlen(buf)-1);
        user->type = KICK_USER;
        if(send(sid,user,2048,0) < 0) {
            perror("send()");
            exit(-1);
        }
        if(recv(sid,&type,sizeof(type),0) < 0) {
            perror("recv()");
            exit(-1);
        }
        if(type == NAME_FAILED) {
            printf("用户名错误!\n");
        }else if(type == SUCCESS) {
            printf("踢人成功!\n");
        }
    }
        printf("\033[0m");

} 
int  id_online(struct user *user){
        printf("\033[0;36m");
    int type;
    user->type = IS_ONLINE;
    if(send(sid,user,2048,0) < 0) {
        perror("send()");
        exit(-1);
    }
    if(recv(sid,&type,sizeof(type),0) < 0) {
        perror("recv()");
        exit(-1);
    }
    printf("type=%d",type);
    return type;
}
void exit_user(struct user *user){
    int type;
    user->type = EXIT_USER;
    if(send(sid,user,2048,0) < 0) {
        perror("send()");
        exit(-1);
    }
    if(recv(sid,&type,sizeof(type),0) < 0) {
        perror("recv()");
        exit(-1);
    }
    if(type == SUCCESS) {
        printf("退出成功!\n");
    }
        printf("\033[0m");
}
void private_chat(struct user *user){
        printf("\033[0;36m");
    char buf[1024];
    char name[10];
    user->type = IS_BAN;
    int type;
    if(send(sid,user,2048,0) < 0) {
        perror("recv()");
        exit(-1);
    }
    if(recv(sid,&type,sizeof(type),0) < 0) {
        perror("send()");
        exit(-1);
    }
    if(type == IS_BAN){
        printf("您已被群主禁言!\n");
        return;
    }
    char num;
    while(1){
        printf("1.发送/2.退出/3.刷新：\n");
        num=fgetc(stdin);
        getchar();
        printf("num=%c",num);
        if(num=='1'){
            printf("请输入要发送信息的用户名：\n");
            fgets(name,10,stdin);
            printf("请输入发送内容：\n");
            fgets(buf,1024,stdin);
            strcpy(user->data,buf);
            user->type = PRIVATE_CHAT;
            memcpy(user->private_name,buf,strlen(buf)-1);
            if(send(sid,user,2048,0) < 0) {
                perror("send()");
                exit(-1);
            } 
        }else if(num=='2'){
            break;
        }else if(num=='3'){
            printf("私聊信息：%d\n",private_num);
            for(int j=0;j<private_num;j++){
                printf("%s",message_g[j]);
            }
        }
        fflush(stdin);
    }
        printf("\033[0m");
}
/*
   void file_transfer(struct user *user){
   }
   */
