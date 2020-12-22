#include "../Include/pub.h"

int main(int argc,char **argv){
    int sid;
    struct sockaddr_in seraddr;
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    socklen_t serlen;

    clilen = sizeof(cliaddr);
    serlen = sizeof(seraddr);
    bzero(&seraddr,serlen);
    bzero(&clilen,clilen);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORT);
    inet_pton(AF_INET,"192.168.75.128",&seraddr.sin_addr.s_addr);

    if ((sid = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    if (bind(sid,(struct sockaddr *)&seraddr,serlen) < 0) {
        perror("bind()");
        exit(-1);
    }
    if (listen(sid,10) < 0) {
        perror("listen()");
        exit(-1);
    }
}
