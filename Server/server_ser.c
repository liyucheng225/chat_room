#include "../Include/pub.h"
#include "./pub.h"
int i=0;
int efd;
int flag=0;//进入聊天室的顺序
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
        printf("%d success\n",clid);
        cid[i].clid=clid;
        ev[i].events = EPOLLIN;
        ev[i].data.fd = clid;
        epoll_ctl(efd,EPOLL_CTL_ADD,cid[i].clid,&ev[i]);
        i++;
    }
}
void database_handle(struct user *user,int clid){
    printf("处理函数\n");
    MYSQL mysql;
    struct quest *que=(struct quest *)malloc(100);
    database_connect(&mysql); 
    int type=0;
    if (user->type == USER_CREAT) {
        //数据库查找函数
        //如果存在此用户，说明已注册，否则存入数据库；
        printf("插入数据\n");
        printf("ps=%s\n",user->passwd);
        printf("que=%s\n",user->question);
        printf("name=%s\n",user->username);
        printf("ans=%s\n",user->answer);
        database_insert_data_to_clientinfo(&mysql,user);//存入数据库 
    }else if (user->type == USER_LOGIN) {
        //数据库查找函数
        printf("type=%d",type);
        type = database_search_user(&mysql,user);
        printf("type=%d",type);
        if(send(clid,&type,sizeof(type),0) < 0) {
            perror("send()");
            exit(-1);
        }
        //如果存在且密码匹配则成功
        //不存在则返回啥失败
    }else if (user->type == USER_CHANGE) {
        memset(que->question,0,20);
        memset(que->answer,0,10);
        printf("type=%d",type);
        type = database_search_question(&mysql,user->username,que);
        printf("type=%d",type);
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
        printf("search name\n");
    }else if (user->type == CHAT) {
        char buf1[2048];
        char buf2[2048];
        sprintf(buf1,"%s:%s",user->username,user->data);
        sprintf(buf2,"我:%s",user->data);
        printf("buf1=%s",buf1);
        printf("buf2=%s",buf2);
        for(int j=0;j<i;j++) {
            if(cid[j].clid > 0) {
                if(cid[j].clid==clid){
                    send(cid[j].clid,buf2,2048,0);
                }
                else{ 
                    send(cid[j].clid,buf1,2048,0);            
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
                printf("禁言=%s,%s",cid[j].name,user->username);
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
                printf("解禁=%s,%s",cid[j].name,user->username);
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
                printf("踢出=%s,%s",cid[j].name,user->username);
                if(strcmp(cid[j].name,user->username)==0) {
                    cid[j].is_ban=0;
                    cid[j].num=0;
                    cid[j].online=0;
                    cid[j].type=0;
                    printf("name=%s,cid=%d",cid[j].name,cid[i].online);
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
                printf("判断在不在现=%s,%d,%d",cid[j].name,cid[j].clid,clid);
                printf("online=%d",cid[j].online);
                if(cid[j].clid==clid&&cid[j].online==0) {
                    type=NAME_FAILED;
                }
            }
        }
        send(clid,&type,sizeof(type),0);
    }
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
        printf("nready=%d\n",nready);
        for (int i=0;i<nready;i++){
            printf("cid=%d",ev_ready[i].data.fd);
            if (recv(ev_ready[i].data.fd,user,2048,0) < 0) {
                perror("recv()");
                exit(-1);
            }
            printf("name=%s\n",user->username);
            database_handle(user,ev_ready[i].data.fd);
        }
    }
    return 0;
}

