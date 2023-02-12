#include "Server.h"
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = \
            std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server() {
    delete acceptor;
}

// void Server::handleReadEvent(int sockfd) {
    
// }

void Server::newConnection(Socket *sock) {
    Connection *cnet = new Connection(loop, sock);
    std::function<void(Socket*)> cb = 
        std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    cnet->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = cnet;
}

void Server::deleteConnection(Socket *sock) {
    Connection *cnet = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete cnet;
}

