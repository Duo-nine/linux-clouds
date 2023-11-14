#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


int main()
{
	int server_socket;
	struct sockaddr_in server_addr;
	int client_socket;
	char buffer[1024]  ={0};
	ssize_t len;

	if( -1 == (server_socket = socket(AF_INET, SOCK_STREAM, 0)) )
	{
		perror("socket");
		return -1;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(60000);
	server_addr.sin_addr.s_addr = INADDR_ANY;		/* IP 0.0.0.0 泛指本机所有可以使用的IP*/

	if( bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) )
	{
		perror("bind");
		return -1;
	}

	if( listen(server_socket, 50) )
	{
		perror("listen");
		return -1;
	}

	/* accept函数用来接收客户端发起的连接请求 没有客户端发起请求时 该函数阻塞 */
	if( -1 == (client_socket = accept(server_socket, NULL, NULL)) )
	{
		perror("accept");
		return -1;
	}
/*
	if( -1 == (client_socket = accept(server_socket, NULL, NULL)) )
	{
		perror("accept");
		return -1;
	}
	else
	{
		printf("Connection establishment.\n");
	}
*/

	while(1)
	{
		memset(buffer, 0x00, sizeof(buffer));
		len = recv(client_socket, buffer, sizeof(buffer), 0);
		if( len < 0 )
		{
			perror("recv");
			break;
		}
		else if( !len )
		{
			printf("Connection close by client.\n");
			break;
		}
		printf("recv : %s\n",buffer);
	}

	close(client_socket);
	close(server_socket);

	return 0;
}

/*
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	用来接收客户端的连接
		@sockfd  - 服务端套接字
		@addr    - 协议地址结构指针,如果需要存储连接的客户端协议地址信息
				   使用该指针指定存储区域首地址
		@addrlen - 指针,该指针指向的内存单元中存储了协议地址信息的大小
	成功返回一个套接字,该套接字用来和刚刚连接好的客户端进行通信
	失败返回-1

*/
