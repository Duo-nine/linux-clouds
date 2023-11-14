#include "contacts.h"

/* 字符串过滤(去掉末尾换行符) */
static void stringFliter(char *str)
{
	if( *(str + strlen(str) - 1) == '\n' )
	{
		*(str + strlen(str) - 1) = 0x00;
	}
}

static void saveNodeData(T_CONT *pdata, FILE *fp)
{
	if( strcmp(pdata->name,"HEAD_NODE") )
	{
		if( fwrite(pdata, 1, sizeof(T_CONT), fp) < 0 )
		{
			ERROR_HANDLE("fwrite");
			return ;
		}
	}
}

/* 姓名字符串(大写转小写) */
static int nameCmp(const char *n1, const char *n2)
{
	char s1[16] = {0}, s2[16] = {0};
	strcpy(s1,n1);
	strcpy(s2,n2);
	for(int i = 0; i < 16;i++)
	{
		if( s1[i] <= 'Z' && s1[i] >= 'A' )
		{
			s1[i] += 32;
		}
		if( s2[i] <= 'Z' && s2[i] >= 'A' )
		{
			s2[i] += 32;
		}
	}
	return strcmp(s1,s2);
}

/* 链表初始化 */
void *linkListInit(T_CONT **pphead)
{
	T_CONT  tmp;
	T_CONT *node = NULL;	//指向最新开辟的内存空间
	T_CONT *p    = NULL;
	FILE *fp     = NULL;
	fp = fopen(SAVE_DATA_PATH,"rb");
	if( !fp )
	{
		ERROR_HANDLE("fopen");
		return NULL;
	}
	
	/* 分配一段动态内存(开辟头节点空间) */
	*pphead = (T_CONT *)malloc(sizeof(T_CONT));
	if( *pphead == NULL )
	{
		ERROR_HANDLE("malloc");
		return NULL;
	}
	strcpy((*pphead)->name,"HEAD_NODE");	//标记头节点
	p = *pphead;	//p指向头节点

	/* 从文件中读取已有的全部联系人数据 */
	while( fread(&tmp, 1, sizeof(T_CONT), fp) == sizeof(T_CONT) )
	{
		node = (T_CONT *)malloc(sizeof(T_CONT));
		*node = tmp;
		p->next = node;
		p = p->next;
	}
	p->next = NULL;
	return p;
}

/* 链表释放 */
void linkListDestory(T_CONT *phead)
{
	T_CONT *p1 = phead, *p2 = NULL;
	FILE *fp = fopen(SAVE_DATA_PATH,"wb");
	if( !fp )
	{
		ERROR_HANDLE("fopen");
		return ;
	}
	while( p1->next )
	{
		p2 = p1;
		p1 = p1->next;
		saveNodeData(p2,fp);
		free(p2);
	}
	saveNodeData(p1,fp);
	free(p1);
	fclose(fp);
}

/* 插入节点 */
void insertNode(T_CONT *phead)
{
	T_CONT        *p1 = phead, *p2 = NULL;	//指针p1指向表头,p2指向空
	T_CONT *new_node  = NULL;				//指向新节点
	

	system("clear");
	new_node = (T_CONT *)malloc(sizeof(T_CONT));	//开辟新节点存储新联系人信息
	if( new_node == NULL )
	{
		ERROR_HANDLE("malloc");
		return;
	}
	/* 编辑联系人信息 */
	printf("Input new contact name         : ");
	fgets(new_node->name,sizeof(new_node->name),stdin);
	stringFliter(new_node->name);
	
	if( !strcmp(new_node->name,"exit") )
	{
		free(new_node);
		return ;
	}
	
	printf("                  phone-number : ");
	fgets(new_node->phone_number,sizeof(new_node->phone_number),stdin);
	stringFliter(new_node->phone_number);

	while( p1->next )
	{
		p2 = p1;		//p2记录p1上一次指向的位置
		p1 = p1->next;	//p1指向下一个节点
		if( nameCmp(p1->name,new_node->name) > 0 )
		{
			/* 插入新节点 */
			p2->next = new_node;
			new_node->next = p1;
			return ;
		}
	}
	p1->next = new_node;
	new_node->next = NULL;
}

void deleteNode(T_CONT *phead)
{
	T_CONT *p1 = phead, *p2 = NULL;
	char buf[16] = {0};
	
	printf("Input contact name you want to delete : ");
	fgets(buf,sizeof(buf),stdin);
	stringFliter(buf);

	while( p1->next )
	{
		p2 = p1;
		p1 = p1->next;
		if( !strcmp(p1->name, buf) )
		{
			p2->next = p1->next;
			free(p1);
			return ;
		}
	}
	printf("Target \"%s\" not found.\n",buf);
	getchar();
}

/* 遍历链表中的节点 */
void printNode(T_CONT *phead){
	T_CONT *p = phead;
	int number = 0;
	system("clear");
	printf("NAME           PHONE-NUMBER\n");
	printf("==============================\n");
	while( p->next != NULL )
	{
		p = p->next;
		printf("%-15s%-11s\n",p->name,p->phone_number);
		number++;
	}
	printf("==============================\n");
	printf("A total of %d contacts\n",number);
	getchar();
};

void changeNode(T_CONT *phead)
{
	printf("%s - %d - %s\n",__FILE__,__LINE__,__func__);
	getchar();getchar();
};

void searchNode(T_CONT *phead)
{
	printf("%s - %d - %s\n",__FILE__,__LINE__,__func__);
	getchar();getchar();
};

