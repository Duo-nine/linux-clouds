#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>



int getMaxValInFds(fd_set *pfds);
int getFdsNumber(fd_set *pfds);
void* threadHandleClient(void* arg);
fd_set fds, tmp_fds;
int ret = 0;
int server_socket;					/* 用来存储接受的客户端套接字 */
/* 主线程 */
int main(int argc, char **argv){

	unsigned short port = 60000;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	int optval = 1;
	
	int client_socket;

	int select_ret;
	char buffer[1024] = {0};
	ssize_t recvlen;
	pthread_t pt;

	if( argc > 2 ){	/* 主函数检查 */
		printf("< usage > : %s [port] \n", argv[0]);
		printf("            Default port is 60000.\n");
		return -1;
	}
	if( argv[1] != NULL ){
		port = atoi(argv[1]);
	}
	if( -1 == (server_socket = socket(AF_INET, SOCK_STREAM, 0)) ){
		perror("socket");
		return -1;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;		/* IP 0.0.0.0 泛指本机所有可以使用的IP*/

	if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ){
		perror("setsockopt");
		return -1;
	}
	if( bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) ){
		perror("bind");
		return -1;
	}
	if( listen(server_socket, 50) ){
		perror("listen");
		return -1;
	}
	system("clear");
	printf("Server established succeed.\n");

	/* 初始化套接字集合 */
	FD_ZERO(&fds);
	FD_SET(server_socket, &fds);	//将监听套接字放入集合中

	struct timeval time_val;
	int nfds;
	
	while(1){
		time_val.tv_sec = 2, time_val.tv_usec = 0;	//设置超时时间为2s
		tmp_fds = fds;	//拷贝一份集合专门用于select检查
		//printf("before select - tmp_fds : %d\n", getFdsNumber(&tmp_fds));
		nfds = getMaxValInFds(&tmp_fds) + 1;
		select_ret = select( nfds, &tmp_fds, NULL, NULL, &time_val);
		if( select_ret < 0 ){
			perror("select");break;
		}else if( !select_ret ){
			//printf("timeout.\n");
		}else{
			if( FD_ISSET(server_socket, &tmp_fds) ){	/* 发现监听套接字被保留在检查后的集合中 说明需要接收客户端的连接 */
				client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
				if( client_socket < 0 ){
					perror("accept");break;
				}
				printf("==== client%02d %-15s : %d connected.  ====\n" \
					,client_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port) );
				FD_SET(client_socket, &fds);	/* 将新连接的客户端套接字放入集合中 */
				pthread_create(&pt, NULL, threadHandleClient, NULL);
			}else{					
				for(int i = server_socket + 1; i < nfds; i++){
					if( FD_ISSET(i, &tmp_fds) ){
						memset(buffer, 0x00, sizeof(buffer));
						recvlen = recv(i, buffer, sizeof(buffer), 0);
						if( recvlen < 0 || !recvlen ){
							getpeername(i, (struct sockaddr *)&client_addr, &addr_len);
							if(!recvlen){
								printf("#### client%02d %-15s : %d disconnected ####\n" \
									,i, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
							}else{perror("recv");}
							FD_CLR(i, &fds);
							close(i);
						}
						else if (*buffer =='/') {
							for (int i = server_socket + 1; i <= ret; i++) {
								//printf("%d\n", i);
								if (-1 == send(i, buffer, sizeof(buffer), 0))
								{
									perror("send");
									break;
								}

							}

						}
						else{
							printf("client%02d : %s\n", i, buffer);
						}
						break;
					}
				}
			}
		}		
		//printf("After select - tmp_fds : %d\n", getFdsNumber(&tmp_fds));
	}
	
	close(server_socket);

	return 0;
}

int getMaxValInFds(fd_set *pfds){

	for(int i = 0 ;i < FD_SETSIZE; i++){
		if( FD_ISSET(i, pfds) ){
			ret = i;
		}
	}
	return ret;
}

int getFdsNumber(fd_set *pfds){
	
	for(int i = 0 ;i < FD_SETSIZE; i++){
		if( FD_ISSET(i, pfds) ){
			ret++;
		}
	}
	return ret;
}
void* threadHandleClient(void* arg) {
	char buffer[1024] = { 0 };
	int i = 4;
	while (1) {
		fgets(buffer, sizeof(buffer), stdin);
		for (; i <= ret;i++) {
			//printf("%d\n", i);
			if (-1 == send(i, buffer, sizeof(buffer), 0))
			{
				perror("send");
				break;
			}
		
		}
		memset(buffer, 0x00, sizeof(buffer));

	/*	if (FD_ISSET(i, &fds)) 
		{
			printf("%d", i);
			if (-1 == send(i, buffer, sizeof(buffer), 0))
			{
				perror("send");
				break;
			}
			else {
				printf("send success\n");
			}
		}
		i++;*/
	}
	_exit(-1);


}


/*
	int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
	函数功能:select用于在给定的时间范围内,检查一个或多个描述符集合中的描述符
			 是否在读\写\异常状态上产生变化
			 并返回有变动描述符数量,同时删除集合中没有变动的描述符
	函数参数:nfds - fds is the highest-numbered file descriptor in any of the three sets, plus 1.
			 readfds   - 该指针指向的集合,select会检查内部的描述符是否有读取动作
			 writefds  - 该指针指向的集合,select会检查内部的描述符是否有写入动作
			 exceptfds - 该指针指向的集合,select会检查内部的描述符是否出现异常
			 timeout   - 该指针指向一个结构,结构中的成员用来描述具体的时间
			 			 写NULL表示不设置时间范围,阻塞的检查,一旦描述符产生变动,立即返回
	返 回 值:失败返回-1,成功返回有变动的描述符个数(超时为0)

	void FD_CLR(int fd, fd_set *set);		从指定的集合中删除描述符
	int  FD_ISSET(int fd, fd_set *set);		检查集合中是否有指定的文件描述符
											存在返回1, 失败返回0
	void FD_SET(int fd, fd_set *set);		向指定的集合中添加描述符
	void FD_ZERO(fd_set *set);				将目标集合清空


数据类型 fd_set 被用来定义一个描述符集合

/usr/include/x86_64-linux-gnu/sys/select.h 中对fd_set的定义

typedef struct{ 
#ifdef __USE_XOPEN
	__fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->fds_bits)
#else
	__fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->__fds_bits)
#endif
} fd_set;

__fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];
x86_64-linux-gnu/sys/select.h:typedef long int __fd_mask;

long int fds_bits[16]

fd_set实际上是一个结构体类型 该结构体中只有唯一一个数据成员
该成员是一个类型为long长度为16的数组

*/

