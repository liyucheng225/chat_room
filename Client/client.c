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
char file[5][1024];
int i=0;
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
        printf("--------------------聊天室--------------------\n");
        printf("  1.用户注册\n");
        printf("  2.用户登陆\n");
        printf("  3.忘记密码\n");
        printf("  4.退出\n");
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
    fflush(stdin);
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
        return;
    }
    //向服务端发送信息
    user_temp->type = USER_CREAT;
    if((relen=send(sid,user_temp,2048,0)) < 0) {
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
    int rebuf;
    char buf[10];
    fflush(stdin);
    bzero(user->username,10);
    printf("请输入用户名：\n");
    memset(buf,0,10); 
    fgets(buf,10,stdin);
    memcpy(user->username,buf,strlen(buf)-1);
    printf("%s",user->username);
    bzero(user->passwd,10);
    printf("请输入密码：\n");
    memset(buf,0,10);
    fflush(stdin);
    fgets(buf,10,stdin);
    memcpy(user->passwd,buf,strlen(buf)-1);
    printf("ps=%s",user->passwd);
    user->type = USER_LOGIN;
    if((relen=send(sid,user,2048,0)) < 0) {
        perror("recv()");
        exit(-1);
    }
    if ((relen = recv(sid,&rebuf,sizeof(rebuf),0)) < 0) {
        perror("recv");
        exit(-1);
    }
    printf("type=%d",rebuf);
    if(rebuf == NAME_FAILED) {
        printf("用户名错误,请重新输入\n");
    }else if(rebuf == PWSD_FAILED) {
        printf("密码错误请,重新输入\n");
    }else if(rebuf == SQL_SUCCESS) {
        printf("验证成功，进入聊天界面\n");
        chat_room(user);
    }
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
        printf("type=%d",rebuf);
        if ((relen = recv(sid,que,100,0)) < 0) {
            perror("recv");
            exit(-1);
        }
        printf("type=%d",rebuf);
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

}
void user_exit(struct user *user){
    int relen;
    user->type = USER_EXIT;
    if((relen=send(sid,user,2048,0)) < 0) {
        perror("recv()");
        exit(-1);
    }
    close(sid);
}
void *recv_select(void *arg){
    char buf[2048];
    int relen;
    while(1){
        if((relen=recv(sid,buf,2048,0)) < 0) {
            perror("recv()");
            exit(-1);
        }
        if(relen > 0){
            strcpy(file[i],buf);
            i++;
            if(i==5){
            for(int j=0;j<i;j++){
                memset(file[j],0,1024);
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
    while(1) {
        num=0;
        printf("--------------------聊天室--------------------\n");
        printf("  1.查看在线人数\n");
        printf("  2.群聊\n");
        printf("  3.文件传输\n");
        printf("  4.禁言\n");
        printf("  5.解禁\n");
        printf("  6.踢人\n");
        printf("  7.私聊\n");
        printf("  7.退出\n");
        printf("  请输入您的选择：\n");
        num=fgetc(stdin);
        getchar();
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
                   //                private_chat(user);
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        case 8:{
                   printf("--------------------聊天室--------------------\n");
                   printf("--------------------退出--------------------\n");
                   printf("请按任意键继续:");
                   fgetc(stdin);
               }
               break;
        default:
               printf("您的输入有误,请重新输入！\n");
               break;
        }
        fflush(stdin);
    }
}
void group_chat(struct user *user){
    if(id_online(user)<0){
        printf("您以被群主强制下线!\n");
        return;
    }
    char num;
    char buf[1024];
    pthread_t thread;
    if(pthread_create(&thread,0,recv_select,0) < 0) {
        perror("pthread_create()");
        exit(-1);
    }
    while(1){
        printf("1.发送/2.退出/(回车)刷新：\n");
        num=fgetc(stdin);
        fgetc(stdin);
        printf("num=%c",num);
        if(num=='1'){
        fflush(stdin);
        printf("请输入发送的内容：\n");
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
        }
        if(num=='2'){
            break;
        }
        if(num=='\n'){
            for(int j=0;j<i;j++){
                printf("%s",file[j]);
            }
        }
    }
}
void online_user(struct user *user){
    int relen;
    user->type = ONLINE_USER;
    if(send(sid,user,2048,0) < 0) {
        perror("recv()");
        exit(-1);
    }
    if(recv(sid,&relen,sizeof(relen),0) < 0) {
        perror("send()");
        exit(-1);
    }
    printf("当前在线人数为：%d\n",relen);

}
void ban_user(struct user *user){
    if(id_online(user)<0){
        printf("您以被群主强制下线!\n");
        return;
    }
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

}
void pick_user(struct user *user){
    if(id_online(user)<0){
        printf("您以被群主强制下线!\n");
        return;
    }
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


}


void kick_user(struct user *user){
    if(id_online(user)<0){
        printf("您以被群主强制下线!\n");
        return;
    }
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

} 
int  id_online(struct user *user){
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
/*
   void file_transfer(struct user *user){
   }
void private_chat(struct user *user){

}*/
