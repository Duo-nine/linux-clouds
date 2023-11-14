#include "msg_handle.h"
#include "fds_manage.h"

void msgHandler(int srcfd, char *msg){
	char *p = msg;
	while( *p == ' ' || *p == '\r' || *p == '\n' ){
		p++;
	}
	if( *p == 0x00 ){	//空消息处理
		return ;
	}else{				//非空消息处理
		p = msg;
		while( *p == ' ' ){
			p++;
		}
		if( *p == '/' ){
			char *p_sktid = p + 1;	//指向id编号的字符
			char *p_msg = p;
			fd_set fds;
			copyFds(&fds);
			while( *p_msg != ':' ){
				p_msg++;
			}
			/* 检查是否目标是否有效 */
			if( !FD_ISSET(atoi(p_sktid), &fds) ){
				if( srcfd == STDOUT_FILENO ){
					write(srcfd, "No such target\n", 15);
				}else{
					send(srcfd, "No such target\n", 15, 0);
				}
				return ;
			}
			sendMsgToTarget(srcfd, atoi(p_sktid), p_msg + 1);
		}else{
			msgBroadcast(srcfd, msg);
		}
	}
}

void msgBroadcast(int srcfd, char *msg){
	fd_set fds;
	int i;
	copyFds(&fds);
	for(i = 0; i < FD_SETSIZE; i++){
		if( FD_ISSET(i, &fds) ){
			sendMsgToTarget(srcfd, i, msg);
		}
	}
}

ssize_t sendMsgToTarget(int srcfd, int destfd, char *msg){
	char buffer[1024] = {0};
	sprintf(buffer ,"client%03d : %s\n", srcfd, msg);
	if( destfd == STDOUT_FILENO ){
		return write(destfd, buffer, strlen(buffer));
	}else{
		return send(destfd, buffer, strlen(buffer), 0);
	}
}

