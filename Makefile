
.DEFAULT_GOAL := all
all: server client

CXXFLAGS += -Wall -g

server: server.cpp src/util.cpp src/Epoll.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp \
		src/EventLoop.cpp src/Server.cpp
	g++ $(CXXFLAGS) -o $@ $^

client: client.cpp src/util.cpp
	g++ $(CXXFLAGS) -o $@ $^

.PHONY: clean 
clean:
	rm -rf client server
