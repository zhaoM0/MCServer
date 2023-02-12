#pragma once 
#include <arpa/inet.h>

class InetAddress {
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();

    void setInetAddr(sockaddr_in _addr, socklen_t _addr_len);
    sockaddr_in getAddr();
    socklen_t getAddrLen();

private:
    struct sockaddr_in addr;
    socklen_t addr_len;
};