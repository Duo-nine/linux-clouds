#include <stdio.h>

struct xxx{
	double a;	// 8	
	short  b;	// 2
	int    c;	// 4
	char   d;	// 1
	long   e;	// 8
};

int main(){

	printf("%lu\n",sizeof(struct xxx));

	return 0;
}
