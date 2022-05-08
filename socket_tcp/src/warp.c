
#include "warp.h"

void sys_err(const char *str){
	perror(str);
	exit(1);
}

int Socket(int domain, int type, int protocol){
	int n = 0;
	n = socket(domain, type, protocol);
	if(n == -1) {
		sys_err("socket error");
		return n;
	}
	return n;
}

int Listen(int sockfd, int backlog){
	int n = listen(sockfd, backlog);
	if(n == -1){
		sys_err("listen error");
		return n;
	}
	return 0;
}


int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
	int n = accept(sockfd, addr, addrlen);
	if(n == - 1){
		sys_err("accept error");
		return n;
	}
	return n;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int n = bind(sockfd, addr, addrlen);
	if(n == -1){
		sys_err("Bind error");
		return n;
	}
	return 0;
}

ssize_t Readn(int fd, void *vptr, size_t n) {
	size_t nleft;
	ssize_t nread;
	char *ptr;

	ptr = vptr;
	nleft = n;

	while(nleft > 0) {
		if((nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR) nread = 0;
			else return -1;
		}else if (nread == 0) break;
		
		nleft -= nread;
		ptr += nread;
	}
	return n - nleft;
}

ssize_t Writen(int fd, const void *vptr, size_t n) {
	size_t nleft;
	ssize_t nwritten;
	const char* ptr;

	ptr = vptr;
	nleft = n;
	while(nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR) nwritten = 0;
			else return -1;
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

static ssize_t my_read(int fd, char *ptr) {
	static int read_cnt;
	static char *read_ptr;
	static char read_buf[100];

	if (read_cnt <= 0) {
again:
		if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR) goto again;
			return -1;
		} else if (read_cnt == 0) return 0;

		read_ptr = read_buf;
	}
	read_cnt--;
	*ptr = *read_ptr++;

	return 1;
}

ssize_t Readline(int fd, void *vptr, size_t maxlen) {
	ssize_t n, rc;
	char 	c, *ptr;
	ptr = vptr;

	for(n = 1; n < maxlen; n++) {
		if ((rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if(c == '\n') break;
		}else if (rc == 0) {
			*ptr = 0;
			return n-1;
		}else return -1;
	}
	*ptr = 0;
	return n;
}

