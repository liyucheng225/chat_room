#include "../Include/pub.h"
#include "./pub.h"
int i=0;
int efd;
int flag=0;//进入聊天室的顺序
struct mesg mesg;
struct epoll_event ev[50];
struct epoll_event ev_ready[50];
void *server_accept(void *arg) {
    int clid;
    efd = epoll_create(50);
    while(1) {
        if((clid = accept(sid,(struct sockaddr *)&cliaddr,(socklen_t *)&clilen)) < 0) {
            perror("accept()");
            exit(-1);
        }
        cid[i].clid=clid;
        ev[i].events = EPOLLIN;
        ev[i].data.fd = clid;
        epoll_ctl(efd,EPOLL_CTL_ADD,cid[i].clid,&ev[i]);
        i++;
    }
}
void database_handle(struct user *user,int clid){
    MYSQL mysql;
    struct quest *que=(struct quest *)malloc(100);
    database_connect(&mysql); 
    int type=0;
    if (user->type == USER_CREAT) {
        database_insert_data_to_clientinfo(&mysql,user);//存入数据库 
    }else if (user->type == USER_LOGIN) {
        type = database_search_user(&mysql,user);
        for(int j=0;j<i;j++){
            if(cid[j].clid > 0){
                if(strcmp(cid[j].name,user->username)==0){
                    if(cid[j].online==USER_ONLINE){
                        break;
                        type=USER_ONLINE;
                    }
                }
            }
        }
        if(send(clid,&type,sizeof(type),0) < 0) {
            perror("send()");
            exit(-1);
        }
    }else if (user->type == USER_CHANGE) {
        memset(que->question,0,20);
        memset(que->answer,0,10);
        type = database_search_question(&mysql,user->username,que);
        if(send(clid,&type,sizeof(type),0) < 0) {
            perror("send()");
            exit(-1);
        }
        send(clid,que,100,0);
        return;
    }else if(user->type == USER_EXIT) {
        for(int j=0;j<i;j++) {
            if(cid[j].clid==clid) {
                cid[j].clid = 0;
                break;
            }
        }    
        close(clid);
    }else if(user->type == NAME_CREAT){
        type = database_search_question(&mysql,user->username,que);
        if(send(clid,&type,sizeof(type),0) < 0) {
            perror("send()");
            exit(-1);
        }
    }else if (user->type == CHAT) {
        memset(mesg.group_mesg,0,2048);
        memset(mesg.group_my_mesg,0,2048);
        sprintf(mesg.group_mesg,"%s:%s",user->username,user->data);
        sprintf(mesg.group_my_mesg,"我:%s",user->data);
        mesg.type=CHAT;
        for(int j=0;j<i;j++) {
            if(cid[j].clid > 0) {
                if(cid[j].clid==clid){
                    send(cid[j].clid,&mesg,4096,0);
                }
                else{ 
                    send(cid[j].clid,&mesg,4096,0);            
                }
            }
        }
    }else if (user->type == ONLINE_USER ){
        int online_num=0;
        for(int j=0;j<i;j++){
            if (cid[j].clid > 0) {
                if(cid[j].online == USER_ONLINE) {
                    online_num++;
                }    
            }
        }
        send(clid,&online_num,sizeof(online_num),0);      
    }else if(user->type == CHAT_ROOM){
        for(int j=0;j<i;j++){
            if (cid[j].clid > 0) {
                if(cid[j].clid == clid&&cid[j].online != USER_ONLINE){
                    if(flag == 0){
                        cid[j].type=ROOM_MASTER;
                    }else{
                        cid[j].type =ROOM_USER; 
                    }
                    cid[j].num = flag++;
                    cid[j].online = USER_ONLINE;
                    strcpy(cid[j].name,user->username);
                    type=SUCCESS; 
                }else if(cid[j].clid == clid&&cid[j].online == USER_ONLINE){
                    type=USER_ONLINE; 
                }
            }
        }
        send(clid,&type,sizeof(type),0);
    }else if(user->type == BAN_USER) {
        type=NAME_FAILED;
        for(int j=0;j<i;j++){
            if(cid[j].clid > 0) {
                if(strcmp(cid[j].name,user->username)==0) {
                    cid[j].is_ban=1;
                    type=SUCCESS;
                }
            }
        }
        send(clid,&type,sizeof(type),0);
    }else if (user->type == IS_MASTER) {
        for(int j=0;j<i;j++){
            if(cid[j].clid > 0) {
                if(cid[j].clid == clid) {
                    type=cid[j].type;
                    break;
                }
            }
        }
        send(clid,&type,sizeof(type),0);
    }else if(user->type == PICK_USER) {
        type=NAME_FAILED;
        for(int j=0;j<i;j++){
            if(cid[j].clid > 0) {
                if(strcmp(cid[j].name,user->username)==0) {
                    cid[j].is_ban=0;
                    type=SUCCESS;
                }
            }
        }
        send(clid,&type,sizeof(type),0);
    }else if(user->type == KICK_USER){
        type=NAME_FAILED;
        for(int j=0;j<i;j++){
            if(cid[j].clid > 0) {
                if(strcmp(cid[j].name,user->username)==0) {
                    cid[j].is_ban=0;
                    cid[j].num=0;
                    cid[j].online=0;
                    cid[j].type=0;
                    bzero(cid[j].name,10);
                    type=SUCCESS;
                }
            }
        }
        send(clid,&type,sizeof(type),0);
    }else if(user->type == IS_ONLINE) {
        type=SUCCESS;
        for(int j=0;j<i;j++){
            if(cid[j].clid > 0) {
                if(cid[j].clid==clid&&cid[j].online==0) {
                    type=NAME_FAILED;
                }
            }
        }
        send(clid,&type,sizeof(type),0);
    }else if(user->type == EXIT_USER){
        type=NAME_FAILED;
        for(int j=0;j<i;j++){
            if(cid[j].clid > 0) {
                if(strcmp(cid[j].name,user->username)==0) {
                    cid[j].is_ban=0;
                    cid[j].num=0;
                    cid[j].online=0;
                    cid[j].type=0;
                    bzero(cid[j].name,10);
                    type=SUCCESS;
                }
            }
        }
        send(clid,&type,sizeof(type),0);
    }else if(user->type == PRIVATE_CHAT){
        memset(mesg.private_mesg,0,2048);
        sprintf(mesg.private_mesg,"%s向你私发了一条信息:%s",user->username,user->data);
        for(int j=0;j<i;j++){
            if(cid[j].clid>0){
                if(strcmp(cid[j].name,user->private_name)==0){
                    mesg.type=PRIVATE_CHAT;
                    send(cid[j].clid,&mesg,4096,0);
                    break;
                }
            }
        }
    }else if(user->type == IS_BAN){
        for(int j=0;j<i;j++){
            if(cid[j].clid>0){
                if(strcmp(cid[j].name,user->private_name)==0){
                    if(cid[j].is_ban==1){
                        type=IS_BAN;
                    }
                    break;
                }
            }
        }
        if(send(clid,&type,sizeof(type),0) < 0){
            perror("send()");
            exit(-1);
        }
    }
        printf("type=%d\n",type);
    database_close_connection(&mysql);  
}
int main()
{   
    int nready = 0;
    struct user *user=(struct user*)malloc(2048);
    pthread_t thread;
    serlen = sizeof(seraddr);
    clilen = sizeof(cliaddr);
    allinit();//初始化函数
    server_connect();//bind服务端函数
    pthread_create(&thread,NULL,server_accept,NULL);
    while(1) {
        nready = 0;
        if ((nready = epoll_wait(efd,ev_ready,50,-1)) <= 0) {
            continue;         
        } 
        for (int i=0;i<nready;i++){
            if (recv(ev_ready[i].data.fd,user,2048,0) < 0) {
                perror("recv()");
                exit(-1);
            }
            database_handle(user,ev_ready[i].data.fd);
        }
    }
    return 0;
}

