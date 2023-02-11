#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <string.h>
#include <unistd.h>

#define READ_BUFFER 1024

Server::Server(EventLoop *_loop) : loop(_loop) {
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonblocking();

    Channel *serv_ch = new Channel(loop, serv_sock->getFd());
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    serv_ch->setCallback(cb);
    serv_ch->enableReading();
}

Server::~Server() { }

void Server::handleReadEvent(int sockfd) {
    char buf[READ_BUFFER];
    while(1) {      // 使用非阻塞I/O，读取客户端buffer，一次读取buf大小的数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes > 0) {
            /* 正常读取 */
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(read_bytes == -1 && errno == EINTR) {
            /* 客户端正常中断，继续读取 */
            printf("continue reading\n");
            continue;
        } else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            /* 非阻塞IO, 数据读取完毕 */
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if(read_bytes == 0) {
            /* EOF, 客户端断开连接 */
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}

void Server::newConnection(Socket *serv_sock) {
    InetAddress *clnt_addr = new InetAddress();     // !mem leaky
    Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
    printf("new client fd %d IP: %s Port %d\n", clnt_sock->getFd(), 
        inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    Channel *clntChannel = new Channel(loop, clnt_sock->getFd());
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
    clntChannel->setCallback(cb);
    clntChannel->enableReading();
}

