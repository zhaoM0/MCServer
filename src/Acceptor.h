#pragma once 
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor {
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket*)>);
private:
    EventLoop *loop;
    Socket *sock;
    Channel *accept_ch;
    std::function<void(Socket*)> newConnectionCallback;
};