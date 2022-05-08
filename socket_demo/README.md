## Introduction

Linux 中的一切都是文件，每个文件都有一个整数类型的文件描述符；socket 也是一个文件，也有文件描述符。使用 `socket()` 函数创建套接字以后，返回值就是一个 `int` 类型的文件描述符。

内核（kernel）利用文件描述符（file descriptor）来访问文件。文件描述符是非负整数。打开现存文件或新建文件时，内核会返回一个文件描述符。读写文件也需要使用文件描述符来指定待读写的文件。

套接字是一个抽象出来的概念，本质上也是一个文件描述符。

涉及到的 主要头文件为 `<sys/socket.h>、<netinet/in.h>`或`<arpa/inet.h>`

Demo主要划分为Server服务器端和Client客户端，基于socket和tcp协议进行进程间的通信，实现字母大小写转换


## socket()

```
int socket(int af, int type, int protocol);
```
- af 为地址族（Address Family），也就是 IP 地址类型，常用的有 AF_INET 和 AF_INET6。AF 是“Address Family”的简写，INET是“Inetnet”的简写。AF_INET 表示 IPv4 地址，例如 127.0.0.1；AF_INET6 表示 IPv6 地址，例如 1030::C9B4:FF12:48AA:1A2B。你也可以使用 PF 前缀，PF 是“Protocol Family”的简写，等价 AF_INET。
- type 为数据传输方式/套接字类型，常用的有 SOCK_STREAM（流格式套接字/面向连接的套接字） 和 SOCK_DGRAM（数据报套接字/无连接的套接字）
- protocol 表示传输协议，常用的有 IPPROTO_TCP 和 IPPTOTO_UDP，分别表示 TCP 传输协议和 UDP 传输协议。

## bind()

```
int bind(int sockfd,  const struct sockaddr, 
        socklen_t addrlen); 
```
bind(): 把一个本地协议地址赋予一个套接字
- 第1个参数`int sockfd`表示绑定的套接字文件描述符。
- 第2个参数` const struct sockaddr` 是一个指向特定协议的地址结构的指针。
- 第3个参数 `socklen_t addrlen` 是该地址结构的长度。对于TCP，调用bind函数可以指定一个端口号，或指定一个IP地址，也可以两者都指定，还可以都不指定。

## listen()

```
int listen(int sockfd, int backlog); 
```
listen(): 监听端口，用在TCP连接中服务器端的角色

对于一个调用listen()进行监听的套接字，操作系统会给这个套接字维护两个**队列**. 队列分为 **未完成连接队列** 和 **已完成连接队列**:

- **未完成队列**： 当客户端发送三次握手第一次SYN请求，服务器会在该队列中创建对应的项（半连接状态），转变为SYN_RCVD，同时给客户端返回二次握手SYN、ACK。
- **已完成队列**：三次握手完成，连接转变为ESTABLISHED状态，每个完成连接的都放在该队列

这也能说明第二个参数 backlog 的曾经意义，即已完成队列和未完成队列的总和不能超过该参数。目前，该参数进一步明确了标识为：指定套接字上内核为之排队的最大已完成连接数。

而后续的accept()函数则负责将已完成队列的客户端连接取出，也务必尽快取出

## Address init
socket通过`struct sockaddr`和`struct sockaddr_in`这两个结构体用来处理网络通信的地址。这结构体用来处理网络通信的地址。目前常用的还是`struct sockaddr_in`。

```
struct sockaddr_in {
   sa_family_t    sin_family; /* address family: AF_INET */
   in_port_t      sin_port;   /* port in network byte order */
   struct in_addr sin_addr;   /* internet address */
};
```
`sin_family`表示协议族，`sin_port`表示端口，`sin_addr`表示32位IP地址
```
bzero(&mysock,sizeof(mysock));  //初始化结构体
mysock.sin_family = AF_INET;  //设置地址家族
mysock.sin_port = htons(800);  //设置端口
mysock.sin_addr.s_addr = inet_addr("192.168.1.0");  //设置地址
# mysock.sin_addr.s_addr = htonl(INADDR_ANY);
```

## accept()

```
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
- `sockfd` 上述listen函数指定的监听socket
- `addr`   请求连接方（即客户端）地址
- `addrlen` 客户端地址长度
- 函数执行成功返回一个新的连接socket，失败返回-1

## connect()

```
int connect(int sockfd, const struct sockaddr *server_addr, socklen_t *addrlen);
```
- `sockfd` socket函数返回一个socket
- `server_addr`  服务端地址
- `addrlen` 服务端地址地址长度
- 函数执行成功返回0，失败返回-1

## Make 

- 进入项目文件夹
- 执行 `make all` 编译生成可执行文件 server.out 和 client.out
- 开启两个shell分别执行server.out以及client.out
- server端口默认为9527

## Error/Abnormal judgment

- 代码中对于接口的返回进行了判断并进行了封装
- 原始函数位于warp.c当中
