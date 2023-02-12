
.DEFAULT_GOAL := all
all: server client

CXXFLAGS += -Wall

server: server.cpp src/util.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp \
		src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp src/Connection.cpp
	g++ $(CXXFLAGS) -o $@ $^

client: client.cpp src/util.cpp
	g++ $(CXXFLAGS) -o $@ $^

.PHONY: clean 
clean:
	rm -rf client server
