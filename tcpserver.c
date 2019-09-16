# include <stdio.h>
# include <netdb.h>
# include <unistd.h>
# include <string.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <memory.h>
# include <signal.h>

int sockfd;
void sig_handler(int signo){
    if(signo == SIGINT){
        printf("server close\n");
        close(sockfd);
        exit(1);
    }
}

void do_service(int fd){
    FILE *a = fopen("q.mp4","r");
    //FILE *b = fopen("b.mp4","w");
    char data[1024];
    int i;
    while(!feof(a)){
        i = fread(&data,1024,1,a);
        if(write(fd,data,i) != i){
            perror("write error");
        };
    }
    /*char data[1024] = "a";
    int size = strlen(data)*sizeof(char);
    write(fd,data,size);*/
    
}

int main(int argc,char *argv[]){
    if(argc < 2 ){
        printf("usage %s #port\n",argv[0]);
        exit(1);
    }
    if(signal(SIGINT, sig_handler)== SIG_ERR){
        perror("signal sigint error\n");
    }

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        printf("socket error");
        exit(1);
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[1]));
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0){
        perror("bind error");
        exit(1);
    }

    if(listen(sockfd,10) < 0){
        perror("listen error\n");
        exit(1);
    }
    

    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    while(1){
        int fd = accept(sockfd, (struct sockaddr*)&clientaddr,&clientaddr_len);
        if(fd < 0){
            perror("accept error\n");
            continue;
        }

    do_service(fd);
    close(fd);
    }
    return 0;
}
