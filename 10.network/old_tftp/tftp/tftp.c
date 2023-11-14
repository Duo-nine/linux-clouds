#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
 
#define N 128



//./exe [服务端IP] [上传/下载] [文件名] [端口号]


void start_download(int sockfd,struct sockaddr_in serveraddr)
{
	char filename[N] = {'\0'};
	printf("请输入要下载的文件：");
	scanf("%s",filename);
 
	int fd;
	int num   = 0;
	int flags = 0;//标记文件是否存在
	ssize_t bytes;
	char text[1024] = {'\0'};
 
	socklen_t addr_len = sizeof(serveraddr);

	//根据tftp协议，拼写要发送的信息
	int text_len = sprintf(text,"%c%c%s%c%s%c",0,1,filename,0,"octet",0 );

	//发送下载文件请求
	if(sendto(sockfd,text,text_len,0,(struct sockaddr *)&serveraddr,addr_len) < 0)
	{
		perror("sendto:");
		exit(1);
	}
 
	while(1)
	{
		if((bytes = recvfrom(sockfd,text,sizeof(text),0,(struct sockaddr *)&serveraddr,&addr_len)) < 0)
		{
			perror("recvfrom:");
			exit(1);
		}
		printf("ok\n");
 
		if(text[1] == 5)
		{
			printf("error:%s\n",text+4);
			return ;
		}
		else if(text[1] == 3)
		{
			if(flags == 0)
			{
				if((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC)) < 0)
				{
					perror("open:");
					exit(1);
				}
				flags = 1;
			}
			if(((ntohs(*(unsigned short *)(text+2))) == num+1 ) && (bytes == 516))
			{
				num = ntohs(*(unsigned short *)(text+2));
				if(write(fd,text+4,bytes-4) < 0)
				{
					perror("write:");
					exit(1);
				}
				text[1] = 4;
 
				if(sendto(sockfd,text,4,0,(struct sockaddr *)&serveraddr,addr_len) < 0)
				{
					perror("sendto:");
					exit(1);
				}
			}
			else if(((ntohs(*(unsigned short *)(text+2))) == num+1 ) && (bytes < 516))
			{
				if(write(fd,text+4,bytes-4) < 0)
				{
					perror("write:");
					exit(1);
				}
 
				text[1] = 4;
 
				if(sendto(sockfd,text,4,0,(struct sockaddr *)&serveraddr,addr_len) < 0)
				{
					perror("sendto:");
					exit(1);
				}
 
				printf("下载完成\n");
				return ;
			}
		}
	}
}
 
int main(int argc,char const *argv[])
{	
	if(argc != 2)
	{
		printf("missing paarameter\n");
		printf("you should input ip\n");
		exit(1);
	}
 
	int socket_fd;
	if((socket_fd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket:");
		exit(1);
	}
	
	struct sockaddr_in ser_addr;
//	int sockAddrLen = sizeof(struct sockaddr_in);
	ser_addr.sin_family 	 = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
	ser_addr.sin_port		 = htons(69);//tftp下载请求端口为69
 
	start_download(socket_fd,ser_addr);
 
	close(socket_fd);
     return 0;
 }

