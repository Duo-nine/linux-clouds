#include <stdio.h>

struct user {
	char           user_name[16];
	unsigned short user_level;
};
typedef struct user user_t;

int main(int argc, char **argv){

	FILE *fp = fopen("data","rb");
	if( !fp ){
		perror("fopen");
		return -1;
	}

	user_t x ;
	x = (user_t){"xxxxxxxx",123};

#if 0
	fwrite(&x,1,sizeof(x),fp);
#elif 1
	printf("brfore read - %s %hu\n",x.user_name,x.user_level);
	fread(&x,1,sizeof(x),fp);
	printf("after  read - %s %hu\n",x.user_name,x.user_level);
#endif

	fclose(fp);

	return 0;
}
