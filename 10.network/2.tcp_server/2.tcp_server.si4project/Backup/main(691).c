#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *udpRecvMessage(void *arg);

/* 主函数 */
int main(int argc, char **argv){

	if( argc != 3 ){
		printf("< usage > : %s [port] [local_port]\n", argv[0]);
		return -1;
	}

	int sockfd;
	struct sockaddr_in target_addr;		/* 通讯目标 */
	struct sockaddr_in local_addr;
	pthread_t pt;
	int optval = 1;
	char buffer[1024] = {0};
	
	if( -1 == (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) ){
		perror("socket");
		return -1;
	}

	memset(&target_addr, 0x00, sizeof(target_addr));
	target_addr.sin_family      = AF_INET;
	target_addr.sin_port        = htons(atoi(argv[1]));
	target_addr.sin_addr.s_addr = inet_addr("192.168.1.255");

	memset(&local_addr, 0x00, sizeof(local_addr));
	local_addr.sin_family      = AF_INET;
	local_addr.sin_port        = htons(atoi(argv[2]));
	local_addr.sin_addr.s_addr = INADDR_ANY;

	/* 启用广播 | 允许重用本地地址和端口 */
	if( setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, &optval, sizeof(optval)) ){
		perror("setsockopt");
		return -1;
	}
	
	if( bind(sockfd, (const struct sockaddr *)&local_addr, sizeof(local_addr)) ){
		perror("bind");
		return -1;
	}

	if( pthread_create(&pt, NULL, udpRecvMessage, &sockfd) ){
		perror("pthread_create");
		return -1;
	}

	system("clear");
	printf("==== UDP local-port : %d ====\n\n", atoi(argv[2]));

	while(1){
		fgets(buffer, sizeof(buffer), stdin);
		if( sendto(sockfd, buffer, sizeof(buffer), 0, \
				(const struct sockaddr *)&target_addr, sizeof(target_addr)) < 0 ){
			perror("sendto");
			break;
		}
	}

	close(sockfd);

	return 0;
}

void *udpRecvMessage(void *arg){
	int sockfd = *(int *)arg;
	char buffer[1024] = {0};
	struct sockaddr_in target_addr;
	socklen_t addr_len = sizeof(target_addr);

	while(1){
		memset(buffer, 0x00, sizeof(buffer));
		if( recvfrom(sockfd, buffer, sizeof(buffer), 0, \
			(struct sockaddr *)&target_addr, &addr_len) < 0 ){
			perror("recvfrom");
			break;
		}
		printf("[%s:%d] : %s\n", \
			inet_ntoa(target_addr.sin_addr), ntohs(target_addr.sin_port),buffer);
	}

	close(sockfd);
	_exit(-1);
}
