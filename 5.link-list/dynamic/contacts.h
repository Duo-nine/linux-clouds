#ifndef _CONTACTS_H
#define _CONTACTS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SAVE_DATA_PATH		"./data/savedata"
#define ERROR_HANDLE(func)	fprintf(stderr,"#Error: File\"%s\"-line(%d)-%s:%s\n" \
							,__FILE__,__LINE__,func,strerror(errno));

/* 联系人 contacts 结构体类型 */
struct contacts
{
	char name[16];
	char phone_number[16];
	struct contacts *next;
};
typedef struct contacts T_CONT;

/* 函数声明 */
void *linkListInit(T_CONT **phead);			//链表初始化
void linkListDestory(T_CONT *phead);	//链表释放
void insertNode(T_CONT *phead);	//插入节点
void deleteNode(T_CONT *phead);	//删除节点
void printNode(T_CONT *phead);	//遍历节点
void changeNode(T_CONT *phead);	//更改节点数据
void searchNode(T_CONT *phead);	//搜索节点

#endif /* _CONTACTS_H */