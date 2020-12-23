#include "../Include/pub.h"
#include "./pub.h"
int i=0;
int efd;
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
        cid[i]=clid;
        ev[i].events = EPOLLIN;
        ev[i].data.fd = clid;
        epoll_ctl(efd,EPOLL_CTL_ADD,cid[i],&ev[i]);
        i++;
    }
}
void database_handle(struct user *user,int cid){
    printf("处理函数\n");
    MYSQL mysql;
    struct quest *que=(struct quest *)malloc(100);
    database_connect(&mysql); 
    int type;
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
        //如果存在且密码匹配则成功
        //不存在则返回啥失败
    }else if (user->type == USER_CHANGE) {
        memset(que->question,0,20);
        memset(que->answer,0,10);
        type = database_search_question(&mysql,user->username,que);
        if(send(cid,&type,sizeof(type),0) < 0) {
            perror("send()");
            exit(-1);
        }
        send(cid,que,100,0);
        bzero(user->username,10);
        bzero(user->passwd,10);
        bzero(user->question,20);
        bzero(user->answer,10);
        if (recv(cid,user,1000,0) < 0) {
                perror("recv()");
                exit(-1);
        }
        database_insert_data_to_clientinfo(&mysql,user);//存入数据库 
    }else if (user->type == USER_LOGIN) {
        //数据库查找函数
        //如果账号存在则显示秘保问题，如果不存在则返回账号查找失败；
    }else if(user->type == USER_EXIT) {
        //客户端退出，cid删除
        //从监控中删除
    }else if(user->type == NAME_CREAT){
        type = database_search_question(&mysql,user->username,que);
        if(send(cid,&type,sizeof(type),0) < 0) {
            perror("send()");
            exit(-1);
        }
        printf("search name\n");
    }else {
        //向客户端发送错误信息；
    }
    database_close_connection(&mysql);  
}
int main()
{   
    int nready = 0;
    struct user *user=(struct user*)malloc(1000);
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
            if (recv(ev_ready[i].data.fd,user,1000,0) < 0) {
                perror("recv()");
                exit(-1);
            }
            printf("name=%s\n",user->username);
            database_handle(user,ev_ready[i].data.fd);
        }
    }
    return 0;
}

