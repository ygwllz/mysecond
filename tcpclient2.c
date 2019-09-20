# include <stdio.h>
# include <string.h>
# include <netdb.h>
# include <sys/socket.h>
# include <unistd.h>
# include <memory.h>
# include <stdlib.h>

int main(int argc,char* argv[]){
    if(argc < 3){
        printf("usage: %s ip port\n",argv[0]);
        exit(1);
    }
    int sockfd;
    //创建客户端套接字
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("socket error");
        exit(1);
        }
    //创建服务端网络通用地址结构体
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    //主机字节序转换为网络字节序char型转换为int型
    serveraddr.sin_port = htons(atoi(argv[2]));
    //将地址族由点分十进制转换为网络字节序并写到结构体中
    inet_pton(AF_INET,argv[1],&serveraddr.sin_addr.s_addr);
    //用connect函数将sockfd与服务端连接
    if(connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0){
        perror("connect error");
        exit(1);
    }

    char buffer[1024];
    memset(buffer,0,sizeof(buffer));
    int size;
    //将客户端发送的信息存到缓存中
    if((size = read(sockfd,buffer,sizeof(buffer))) < 0){
        perror("read error");
    }
    //把缓存中内容输出到屏幕上
    if((write(STDOUT_FILENO,buffer,size)) != size){
        perror("write error");
    }
    close(sockfd);
    return 0;
}
