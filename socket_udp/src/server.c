#include "warp.h"

#define SERV_PORT 8000

int main(int argc, char *argv[]){
	struct sockaddr_in serv_addr, client_addr;
	socklen_t client_addr_len;

	int sockfd;
	char buf[BUFSIZ];
	char str[INET_ADDRSTRLEN];
	int i, n;

	sockfd = Socket(PF_INET, SOCK_DGRAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(SERV_PORT);

	bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	
	printf("Accepting connections ...\n");
	while (1) {
		client_addr_len = sizeof(client_addr);
		n = recvfrom(sockfd, buf, BUFSIZ, 0, (struct sockaddr *)&client_addr, &client_addr_len);
		if (n == -1) sys_err("recvfrom error");
		
		printf("received from %s at PORT %d\n",
				inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)),
				ntohs(client_addr.sin_port));
		for(i = 0; i < n; i++) buf[i] = toupper(buf[i]);

		n = sendto(sockfd, buf, n, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
		if (n == -1) sys_err("sendto error");
	}
	close(sockfd);

	return 0;

}

