#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

#define DEVICE_BUTTN_PATHNAME	"/dev/input/event3"

/* 主函数 */
int main(){

	int btn_fd;
	struct input_event input_buffer;

	if( -1 == (btn_fd = open(DEVICE_BUTTN_PATHNAME, O_RDONLY)) ){
		perror("open");
		return -1;
	}

	while(1){
		if( read(btn_fd, &input_buffer, sizeof(input_buffer)) == -1 ){
			perror("read");
			break;
		}
		printf("button statu : %d \n", input_buffer.value);
	}

	close(btn_fd);

	return 0;
}
