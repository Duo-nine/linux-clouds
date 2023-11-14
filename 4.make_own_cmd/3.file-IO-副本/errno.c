#include <stdio.h>
#include <errno.h>
#include <string.h>

#define FILENAME	"txt"

int main(){

	FILE *fp = fopen("","");
	if( !fp ){
		printf("fopen \"%s\" failed : %s\n",FILENAME,strerror(errno));
		return -1;
	}

	fclose(fp);

	return 0;
}
