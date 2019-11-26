#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mysql_util.h"
#define PORT 1314
#define BACK_LOG 10
int parse_arg(char *str, char args[][32])
{
	char *ptr,*retptr;
    int i=0;
	
	ptr = str;
 	printf("str:%s\n", str);
	
    while ((retptr=strtok(ptr, ",")) != NULL) {
		if(i < 4)
		{
			memcpy(args[i],retptr,strlen(retptr));
			printf("input args out of range\n");
		}
		
		printf("substr[%d]:%s\n", i, retptr);

		i++;
        ptr = NULL;
    }
 	
    return i;
}
int main(){
    int listenfd,connectfd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    pid_t childpid;
    socklen_t addrlen;
    char buff[4096];  
	
	if (init_mysql())
	{
        print_mysql_error(NULL);
		exit(1);
	}
	
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1){
        perror("socker created failed");
        exit(0);
    }
    int option;
    option = SO_REUSEADDR;
    setsockopt(listenfd,SOL_SOCKET,option,&option,sizeof(option));
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenfd,(struct sockaddr *)&server,sizeof(server)) == -1){
       	perror("Bind error!");
       	exit(1);
    }
    if(listen(listenfd,BACK_LOG) == -1){
       	perror("listend error");
       	exit(1);
    }
    printf("waiting for clinet's request.....\n");
    while(1){
    		int n;
    		addrlen = sizeof(client);
    		connectfd = accept(listenfd,(struct sockaddr*)&client,&addrlen);
    		if(connectfd == -1){
        		perror("accept error");
        		exit(0);
    		}else{
    			printf("client connected\n");
    		}
    		if((childpid = fork()) == 0){	
    			close(listenfd);
    			printf("client from %s\n",inet_ntoa(client.sin_addr));
    			//memset(buff,'\0',sizeof(buff));	
    			printf("ready to read\n");
    			while((n = read(connectfd, buff, 4096)) > 0){
					char args[4][32]={0};
					int len = 0;
    				buff[n] = '\0';
    				printf("recv msg from client: %s\n",buff);
					len = parse_arg(buff,args);
					/*
					printf("args:%d\n", len);
					for(int i=0;i<4;i++){
						printf("after parse [%d]:%s\n", i, args[i]);
					}
					*/
					/*If args is 4 add to mysql*/
					if(len == 4)
					{
						add_thinfo(args[0],args[1],args[2],args[3]);
					}
					else
					{
						printf("Invalid args number:%d\n",len);
					}
    			}
    			printf("end read\n");
                exit(0);
    		}else if(childpid < 0)
    			printf("fork error: %s\n",strerror(errno));
    		close(connectfd);
	}
	deinit_mysql();
    return 0;
}
