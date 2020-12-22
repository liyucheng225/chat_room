#include "pub.h"

int main()
{
    int sid, cid, max_fd;
    struct sockaddr_in seraddr, cliaddr;
    char buf[BUFSIZE];
    fd_set rdfs, tempfs;
    socklen_t clilen;
    struct timeval tv;
    tv.tv_sec = 6;
    int num;
    int relen;
    clilen=sizeof(cliaddr);


    sid = socket(AF_INET, SOCK_STREAM, 0);

    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(8000);
    inet_pton(AF_INET,"192.168.75.128",&seraddr.sin_addr.s_addr );

    bind(sid, (struct sockaddr *)&seraddr, sizeof(seraddr));
    listen(sid, 10);

    memset(buf, 0, sizeof(buf));
    FD_ZERO(&rdfs);
    FD_SET(0, &rdfs);
    FD_SET(sid, &rdfs);
    max_fd = sid;

    while(1)
    {
        tv.tv_sec=10;
        memset(buf,0,sizeof(BUFSIZE));
        num=0;
        tempfs = rdfs;
        printf("selecting...\n");

        if((num = select(max_fd + 1, &tempfs, NULL, NULL, &tv))== -1)
        {
            perror("select failed!\n");
            exit(-1);
        }
        if(num == 0)
        {
            printf("waiting...\n");
            printf("number is : %d\n" , num);
            continue;
        }else {
            printf("select success...\n");
            if (FD_ISSET(0,&tempfs)) {
                fgets(buf,sizeof(buf),stdin);
                printf("buf=%s",buf);
            }
            if (FD_ISSET(sid,&tempfs)) {
                if ((cid=accept(sid,(struct sockaddr *)&cliaddr,(socklen_t *)&clilen)) < 0) {
                    perror("accept()");
                    exit(-1);
                }
                while(1){
                    if ((relen = recv(cid,buf,sizeof(buf),0)) <0 ) {
                        perror("recv()");
                        exit(-1);
                    }
                    if(relen > 0) {
                        printf("客户端发来消息是：%s",buf);
                    }
                }
                close(cid);
            }    
        }
        FD_ZERO(&rdfs);
        FD_SET(0, &rdfs);
        FD_SET(sid, &rdfs);
    }
    return 0;
}
