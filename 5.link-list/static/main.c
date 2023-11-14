#include <stdio.h>

/* 通讯录
 * 程序运行 初始化:从文件中读取联系人信息存入链表 打印菜单 [增删改查 退出]
 * 退出时 将联系人信息存入文件
 */

/* contacts - 结构体类型 存储联系人信息 */
struct contacts{
	char name[16];
	char phone_number[12];
	struct contacts *next;
};

/* 函数声明 */
void printLinkList(struct contacts *phead);

/* 主函数 */
int main(){

	struct contacts head;
	struct contacts x1 = {"Alex", "111111"};
	struct contacts x2 = {"Bob",  "222222"};
	struct contacts x3 = {"Clis", "333333"};
	struct contacts x4 = {"David","444444"};

	struct contacts new_node = {"new_node","787878"};

	/* 链 */
	head.next = &x1;
	x1.next   = &x2;
	x2.next   = &new_node;
	new_node.next = &x4;
	x3.next   = &x4;
	x4.next   = NULL;

	printLinkList(&head);

	return 0;
}

/* 遍历链表
 * @param - phead : 头指针
 */
void printLinkList(struct contacts *phead){

	struct contacts *p = phead;	/* 使用指针p 遍历整个链表 */

	/* 当指针p指向的节点中 next成员数据不为空(该节点后还有节点) */
	while( p->next != NULL ){
		/* 让指针p指向下一个节点的首地址 */
		p = p->next;
		printf("Name : %-5s\tPhone-number : %s\n",p->name,p->phone_number);
	}
}
