# include <stdio.h>
# include <string.h>
# include <sys/socket.h>
# include <stdlib.h>
# include <unistd.h>
# include <memory.h>
# include <netdb.h>

int main(int argc, char *argv[]){
    if(argc < 3){
        printf("usage: %s ip port\n",argv[0]);
        exit(1);
    }

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        perror("socket error");
        exit(1);
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET,argv[1],&serveraddr.sin_addr.s_addr);

    if(connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0){
        perror("connect error");
        exit(1);
    }
    socklen_t serveraddr_len = sizeof(serveraddr);
    int fd = accept(sockfd, (struct sockaddr*)&serveraddr,&serveraddr_len);


    char buffer[1024];
    FILE *b = fopen("tcp.mp4","w");
    memset(buffer,0,sizeof(buffer));
    int size;
    if((size = read(sockfd,buffer,sizeof(buffer))) < 0){
        perror("read error");
        }
    else{
        fwrite(&buffer,1024,1,b);
    }
    if(write(STDOUT_FILENO,buffer,size) != size){
        perror("write error");
    }
    if(argv[3] == "get"){
        write()
    }
    if(argv[3] == "put"){
        FILE* a = fopen("argv[4]","r");
        int i,size;
        char data[1024];
        while(!feof(a)){
            i = fread(&data,1024,1,a);
            if((size =(write(fd,data,i)) < 0){
                perror("write error");       
            }
        }
    }
    
    close(sockfd);
    return 0;
}
