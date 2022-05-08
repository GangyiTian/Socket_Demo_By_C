#include "warp.h"

#define SERV_PORT 9527

int main(int argc, char* argv[]){
	int cfd;
	int conter = 10;
	char buf[BUFSIZ];
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);

	inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cfd == -1) sys_err("socket_error");

	int ret = connect(cfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if(ret != 0) sys_err("connect_error");

	while(--conter){
		write(cfd, "hello\n", 6);
		ret = read(cfd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, ret);
		sleep(1);
	}

	close(cfd);
	return 0;
}
