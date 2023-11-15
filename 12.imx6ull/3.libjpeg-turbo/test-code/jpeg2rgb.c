#include "myfb.h"
#include "jpeg2rgb.h"

void displayJpegimage2LCD(const char *filename){

	struct jpeg_decompress_struct cinfo;	//解压缩对象
	struct jpeg_error_mgr jerr;
	FILE *infile;
	unsigned char *buffer;
	int row_stride;		//存储一行象素点所占用的字节数

	/* Step 1. Allocate and initialize a JPEG decompression object. */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);		//初始化解压缩对象

	/* Step 2. Specify the source of the compressed data (eg, a file). */
    if ((infile = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        return ;
    }
    jpeg_stdio_src(&cinfo, infile);

	/* Step 3. Call jpeg_read_header() to obtain image info. */
	jpeg_read_header(&cinfo, TRUE);

	/* Step 4. Set parameters for decompression */

	/* Step 5. jpeg_start_decompress(...) */
	jpeg_start_decompress(&cinfo);
	printf("image width  : %u px\n", cinfo.image_width);
	printf("image height : %u px\n\n", cinfo.image_height);
	printf("output width  : %u px\n", cinfo.output_width);
	printf("output height : %u px\n\n", cinfo.output_height);

	/* Step 6: while (scan lines remain to be read) */
	/*           jpeg_read_scanlines(...); */

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (unsigned char *)malloc(row_stride);
	if( buffer == NULL ){
		perror("malloc");
		return;
	}

	int i;
	while (cinfo.output_scanline < cinfo.output_height) {
		printf("output_scanline : %u\n", cinfo.output_scanline);
		jpeg_read_scanlines(&cinfo, &buffer, 1);
		//put_scanline_someplace(buffer[0], row_stride);
	}

	

}
