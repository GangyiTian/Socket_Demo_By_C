#include "warp.h"

#define SERV_PORT 9527

int main(int argc, char *argv[]){
	int lfd = 0, cfd = 0, ret;
	char buf[BUFSIZ], client_IP[1024];
	struct sockaddr_in serv_addr, clit_addr;
	socklen_t clit_addr_len;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	lfd = Socket(AF_INET, SOCK_STREAM, 0);
	if(lfd == -1) {
		sys_err("socket_error");
	}
	Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	Listen(lfd, 128);

	clit_addr_len = sizeof(clit_addr);
	cfd = Accept(lfd, (struct sockaddr *)&clit_addr, &clit_addr_len);
	printf("Establish client---->ip:%s port:%d\n", 
			inet_ntop(AF_INET, &clit_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)),
			ntohs(clit_addr.sin_port));
	while(1){
		ret = read(cfd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, ret);
		for(int i = 0; i < ret; i++){
			buf[i] = toupper(buf[i]);
		}	
		write(cfd, buf, ret);
	}

	close(cfd);
	close(lfd);
	return 0;
}

