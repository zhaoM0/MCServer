#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <string.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_sock) :
    loop(_loop), sock(_sock), ch(nullptr) 
{
    ch = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    ch->setCallback(cb);
    ch->enableReading();
}

Connection::~Connection() {
    delete ch;
    delete sock;
}

void Connection::echo(int sockfd) {
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
            deleteConnectionCallback(sock); // 调用Server中管理的删除函数
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb) {
    deleteConnectionCallback = _cb;
}