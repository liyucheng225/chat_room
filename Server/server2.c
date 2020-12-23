#include "../Include/pub.h"
#include "./pub.h"
void server_connect() {
    if ((sid = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    if (bind(sid,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0) {
        perror("bind()");
        exit(-1);
    }
    if (listen(sid,10) < 0) {
        perror("listen()");
        exit(-1);
    }

}
void allinit(){
    clilen = sizeof(cliaddr);
    serlen = sizeof(seraddr);
    bzero(&seraddr,serlen);
    bzero(&clilen,clilen);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(PORT);
    inet_pton(AF_INET,"192.168.75.128",&seraddr.sin_addr.s_addr);

}
