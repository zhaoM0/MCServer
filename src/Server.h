#pragma once 

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

class Server {
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket*);
    void deleteConnection(Socket*); 
private:
    EventLoop *loop;        // 事件循环
    Acceptor *acceptor;     // 用于接受新的TCP连接
    std::map<int, Connection*> connections;  // 管理所有的TCP连接
};