#include "warp.h"

#define SERV_PORT 9000
int main(int argc, char *argv[]){
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(SERV_PORT);

	int listen_fd = socket(PF_INET, SOCK_STREAM, 0);
	bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	int ret = listen(listen_fd, 1);
	printf("%d\n", listen_fd);
	sleep(5);
	printf("%d\n", listen_fd);
	sleep(30);

	close(listen_fd);
	return 0;

}

