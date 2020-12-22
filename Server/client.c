#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFLEN 100
int main()
{
    int sockfd;
    char buf[256];
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    inet_pton(AF_INET,"192.168.75.128",&servaddr.sin_addr.s_addr );
    servaddr.sin_port=htons(8000);

    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    printf("socket success!\n");

    if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0){
        perror("concect()");
        exit(-1);
    }
    printf("connect success!\n");
    while(1) {
        fgets(buf,sizeof(buf),stdin);
        if (send(sockfd,buf,sizeof(buf),0) < 0) {
            perror("send()");
            exit(-1);
        }
        memset(buf,0,256);
    }
    return 0;
}
