#include <stdio.h>

int main(){

	FILE *fp = fopen("./data","r");
	if( !fp ){
		perror("fopen");
		return -1;
	}

	int a = 0, b = 0;

	fscanf(fp,"%d%d",&a,&b);

	printf("a = %d , b = %d\n",a,b);

	fclose(fp);

	return 0;
}
