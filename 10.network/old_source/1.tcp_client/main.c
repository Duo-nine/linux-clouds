#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


/*
	[已OK]1.使用线程进行全双工通信
	[已OK]2.当连接被服务端关闭时,打印"Connection closed by foreign host."
*/

//全局变量
char buffer[1024] = {0};	//用来存放收发数据
int client_socket;			//客户端套接字

//线程处理函数,用于全双工通信
void *threadHandler(void *arg)
{
	while(1)
	{
		fgets(buffer, sizeof(buffer), stdin);
		if( write(client_socket, buffer, sizeof(buffer)) < 0 )
		{
			perror("write");
			break;
		}
		//printf("send : %s\n", buffer);
		memset(buffer, 0x00, sizeof(buffer));//清除上一次数据内容
	}
	close(client_socket);//关闭客户端套接字
	return 0;
}

//主函数
int main(int argc, char **argv)//argv数组存放用户在控制终端输入的具体参数
{
	
	/* 要连接的服务端IP和端口号由主函数参数传入 */
	if( argc != 3 )
	{
		printf("<usage> : %s [ipv4] [port]\n", argv[0]);//./要执行的程序 IPv4地址 端口号
		return -1;
	}

	struct sockaddr_in target;	//目标的internet协议地址结构

//创建线程，用于全双工通信

	pthread_t pt;	//存放线程的标识符
	
	if( pthread_create(&pt, NULL, threadHandler, NULL) )
	{
		perror("pthread_create");
		return -1;
	}

	//printf("pt = %lu\n",pt);//打印线程标识符
	//strcpy(buffer, "Hello!");//将hello！写入buffer

//1.初始化套接字
	if( -1 == (client_socket = socket(AF_INET, SOCK_STREAM, 0)) )
	{
		perror("socket");
		return -1;
	}

//2.连接服务端
	memset(&target,0x00,sizeof(target));//清除上一次收发数据
	target.sin_family      = AF_INET;			  //地址族，AF_INET，2bytes
	target.sin_port        = htons(atoi(argv[2]));//端口号是无符号短整型u16,2bytes，
												  //所以需要转换格式，
												  //htons()功能：转换为u16
	target.sin_addr.s_addr = inet_addr(argv[1]);  //IPv4地址,4bytes
	if( connect(client_socket, (const struct sockaddr *)&target, sizeof(target)) )
	{
		perror("connect");
		return -1;
	}

	int x;//用来接收read返回值
//3.数据传输
	while(1)
	{
		//sleep(1);
		//printf("%s\n",buffer);
		x = read(client_socket, buffer, sizeof(buffer));
		if( x < 0 )
		{
			perror("read");
			break;
		}
		else if(x == 0)
		{
			printf("Connection closed by foreign host.\n");
			break;
		}
		printf("recv data is : %s\n", buffer);
		memset(buffer, 0x00, sizeof(buffer));

	}

	close(client_socket);//关闭套接字初始化
	
	return 0;
}

/*
====================================================================================
函数①
	int socket(int domain, int type, int protocol);
 	功能：初始化套接字
 	参数：@domain			协议族
 		  @type			套接字类型
 		  @protocol		如果套接字设置没有由前两个参数完全指定,
 							那么套接字设置将由此参数决定；
 						如果前两个参数已经填写好,那么此参数写0
====================================================================================
函数②
 	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	功能：客户端向服务端发起连接请求(协议族/地址族 , IP地址 , 端口号)
	参数：@sockfd			客户端套接字
		  @addr			结构体指针,连接时将从这个指针指向的区域中寻找目标IP端口号以及协议族
		  @addrlen		协议地址结构长度(字节)
====================================================================================
函数③
	in_addr_t inet_addr(const char *cp);
	功能：将点分十进制形式的IPv4地址转换为无符号32位网络字节序的数值
====================================================================================

通用地址结构
struct sockaddr
{
	u_short sa_family; 	// 地址族 , AF_xxx
	char sa_data[14]; 	// 14 字节协议地址
};

Internet 协议地址结构
struct sockaddr_in
{
	u_short sin_family; 		// 地址族，AF_INET，2bytes
	u_short sin_port; 			// 端口，2bytes
	struct in_addr sin_addr; 	// IPV4 地址，4bytes
	char sin_zero[8]; 			// 8bytes unused作为填充
};

IPv4 地址结构
struct in_addr
{
	in_addr_t s_addr; 	//u32 network address
};

*/
