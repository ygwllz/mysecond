# include <stdio.h>
# include <string.h>
# include <netdb.h>
# include <sys/socket.h>
# include <unistd.h>
# include <memory.h>
# include <stdlib.h>
# include <signal.h> 


int  sockfd;
//信号处理函数
void sig_handler(int signo){
    if(signo == SIGINT){
        printf("server close\n");
        close(sockfd);
        exit(1);
    }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("lack data\n");
        exit(1);
    }
    //登记信号
    if(signal(SIGINT,sig_handler) == SIG_ERR){
        perror("signal error\n");
        exit(1);
    }
    //创建套接字，并设置套接字选项
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0){
        perror("sock error\n");
        exit(1);
    }
    //新建服务端通用地址结构体
    struct sockaddr_in serveraddr;
    memset(&serveraddr,0,sizeof(serveraddr));
    //登记服务端通用地址结构体的成员
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[1]));
    //把服务端ip地址设为监听一台主机上所有ip地址，这样服务端就不局限于一个ip地址
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    //把服务端套接字和网络地址、端口绑定
    if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0){
        perror("bind error\n");
        exit(1);
    }
    //指定端口监听，设置排队队列长为10
    if(listen(sockfd,10) < 0){
        perror("listen error\n");
        exit(1);
    }
    //新建客户端通用地址结构体
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    //
    while(1){
        //获得队列中的连接，成功返回新的描述符用来和客户端进行读写操作
        int sockfd2 = accept(sockfd,(struct sockaddr*)&clientaddr,
                 &clientaddr_len);
        if(sockfd2 < 0){
            perror("accept error\n");
            continue;
        }
        //发送信息
        char data[1024];
        printf("请输入你想发送的信息\n");
        scanf("%s",&data);
        int i = strlen(data) * sizeof(char);
        write(sockfd2,data,i);
        close(sockfd2);
    }
    return 0;
}
