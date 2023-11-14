#ifndef _MSG_HANDLE_H
#define _MSG_HANDLE_H

#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

void msgHandler(int srcfd, char *msg);
void msgBroadcast(int srcfd, char *msg);
ssize_t sendMsgToTarget(int srcfd, int destfd, char *msg);

#endif /* _MSG_HANDLE_H */