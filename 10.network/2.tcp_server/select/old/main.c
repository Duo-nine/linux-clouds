#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *threadHandleClient(void *arg);

int main(int argc, char **argv){

	unsigned short port = 60000;
	int server_socket;
	struct sockaddr_in server_addr;
	int optval = 1;
	int new_client;					/* 用来存储接受的客户端套接字 */
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

	/* 每次连接一个新客户端 为了接收其发送的信息 创建一个线程 */
	while(1){
		if( -1 == (new_client = accept(server_socket, NULL, NULL)) ){
			perror("accept");
			break;
		}else{
			if( pthread_create(&pt, NULL, threadHandleClient, &new_client) ){
				perror("pthread_create");
				close(new_client);
			}
		}
	}

	close(new_client);

	return 0;
}

/* threadHandleClient - 线程处理函数 - 接收已连接客户端发送的消息
   @arg - 指向新连接至服务端的客户端套接字首地址	*/
void *threadHandleClient(void *arg){
	int client_socket = *(int *)arg;
	char buffer[1024] = {0};
	ssize_t len;
	struct sockaddr_in client_addr;	/* 用来存储接收的客户端协议地址信息 */
	socklen_t addr_len = sizeof(client_addr);

	getpeername(client_socket, (struct sockaddr *)&client_addr, &addr_len);
	printf("==== client%02d %-15s : %d connected.  ====\n" \
	,client_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port) );
	
	while (1){
		memset(buffer, 0x00, sizeof(buffer));
		len = recv(client_socket, buffer, sizeof(buffer), 0);
		if( len > 0 ){
			printf("client%02d : %s\n", client_socket, buffer);
		}else if(!len) {
			printf("#### client%02d %-15s : %d disconnected ####\n" \
				,client_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
			break;
		}else{perror("recv");break;}
	}
	close(client_socket);
	pthread_detach(pthread_self());
	pthread_exit(NULL);
}

