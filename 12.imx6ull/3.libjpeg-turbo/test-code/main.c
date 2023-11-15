#include "myfb.h"
#include "jpeg2rgb.h"

extern void *p_fbmem;
extern size_t fbmem_size;

/* 主函数 */
int main(int argc, char **argv){

	if( argc != 2 ){
		printf("<usage> : %s [jpeg pathname].\n", argv[0]);
		return -1;
	}

	if( fbInit() ){
		fprintf(stderr, "Device \"%s\" initialized failed.\n", basename(DEVICE_FB_PATHNAME));
		return -1;
	}

	fbClean(BLACK);

	displayJpegimage2LCD(argv[1]);

	//fbDisplayImage(512, 300, gImage_ASCII_888, 921, 600);

	//fbShowImage(0, 0,gImage_ASCII_888, 921, 600);

	fbDestory();
	return 0;
}

/* 
	位深 : 一个像素点占用多少位来存储颜色数据

	
*/
